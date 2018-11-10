/**
  ******************************************************************************
  * @file    opentherm.c
  * @author  turchenkov@gmail.com
  * @version V0.0.1
  * @date    07-Nov-2018
  * @brief   This file contains opentherm protocol functions
  ******************************************************************************
  */

#include "config.h"
#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include "opentherm.h"
#include "message_table.h"

/* global variables */


/* static functions prototypes */


/**
  * initializes the MV array
  * @param pmv the pointer to MV array
  * @param pMVArrLen lenthth of the array
  * @return -1 if error, 0 otherwize
  *
  */
int32_t OPENTHERM_InitMV(tMV (*const pMVArr)[], size_t MVArrLen)
{
	if ((pMVArr == NULL) || (MVArrLen == 0)) {
		return -1;
	}

	if (MVArrLen > MSG_TBL_LENGTH) {
		MVArrLen = MSG_TBL_LENGTH;
	}

	for (size_t i = 0; i < MVArrLen; i++) {
		// initialize only first MsgTblLength MVs
		tMV *p = &(*pMVArr)[i];
		p->LD_ID = messagesTbl[i].msgId;
		p->Ctrl = No;
		p->ValChanged = Yes;
		p->TimeStamp = getTime();
		p->Quality = QUALITY_VALIDITY_QUESTIONABLE;
		if (messagesTbl[i].msgDataType1 == f88) {
			p->MV_type = floatMV;
			p->Val.fVal = 0.0;
			p->MV_Unit = messagesTbl[i].MV_Unit1;
			p->MV_Shift.fVal = 0.0f;
			p->MV_Scale.fVal = 1.0f;
			p->MV_Aperture.fVal = 0.0f;
			p->Lowest.fVal = messagesTbl[i].Lowest.fVal;
			p->Highest.fVal = messagesTbl[i].Highest.fVal;
		} else {
			p->MV_type = intMV;
			p->Val.iVal = 0;
			p->MV_Unit = messagesTbl[i].MV_Unit1;
			p->MV_Shift.iVal = 0;
			p->MV_Scale.iVal = 1;
			p->MV_Aperture.iVal = 0;
			p->Lowest.iVal = messagesTbl[i].Lowest.iVal;
			p->Highest.iVal = messagesTbl[i].Highest.iVal;
		}
	}
	return 0;
}

// stub function
tTime getTime(void)
{
	tTime retVal;
	struct timespec spec;
	clock_gettime(CLOCK_REALTIME, &spec);
	retVal.Seconds = spec.tv_sec;
	retVal.mSeconds = spec.tv_nsec / 1000000u;
	return retVal;
}

/**
  * converts f88_t value to float
  * @param val input value
  * @return float value
  */
float F88ToFloat(f88_t val)
{
	float retVal;
	if (val.int_part & 0x80u) {
		retVal = -(0x10000 -
			   (((uint16_t)val.int_part << 8) + val.frac_part)) /
			 256.0f;
	} else {
		retVal = (((uint16_t)val.int_part << 8) + val.frac_part) /
			 256.0f;
	}
	return retVal;
}

/**
  * converts float to f88_t
  * @param val input value
  * @return f88_t value
  */
f88_t FloatTof88(float val)
{
	f88_t retVal;
	// saturation
	val = (val > 127.996094f) ? 127.996094f : val;
	val = (val < -128.000000f) ? -128.000000f : val;
	int32_t a = (int32_t)(val * 256);
	retVal.int_part = (uint8_t)(a >> 8u);
	retVal.frac_part = (uint8_t)(a);
	return retVal;
}

/**
  * checks is val parity even
  * @param val input value
  * @return 0 even, -1 otherwize
  */
int CheckParity32(uint32_t val)
{
	uint8_t onesCount = 0;
	for (size_t j = 0; j < 32U; j++) {
		if ((val & 1u) == 1u) {
			onesCount++;
		}
		val = (val >> 1);
	}
	if ((onesCount & 1u) == 1u) {
		return -1;
	}
	return 0;
}

/**
  * Puts data from MV to the data field(s) to the message
  * @param pMV pointer to the MV
  * @param pmsg pointer to the message
  * @return openThermResult_t as result
  */
openThermResult_t OPENTHERM_PutDataToMsg(tMV *const pMV, uint32_t *const pmsg)
{
	openThermResult_t retVal = OPENTHERM_ResOK;
	if ((pMV == NULL) || (pmsg == NULL)) {
		retVal = OPENTHERM_ResNullPtr; /* bad msgId */
		goto fExit;
	}

	/* recast pmsg */
	openThermFrame_t *const outFrame = (openThermFrame_t *const)pmsg;
	outFrame->dataVal.dataValueU16 = 0u;

	const opentThermMsg_t *const pt = GetMessageTblEntry(pMV->LD_ID);
	if (pt == NULL) {
		outFrame->byte1.msgType = MSG_TYPE_UNKN_DATAID;
		retVal = OPENTHERM_ResBadMsgId; /* bad msgId */
		goto fExit;
	}

	outFrame->dataID = pMV->LD_ID;

	if (pMV->MV_type == intMV) {
		switch (pt->msgDataType1) {
		case u8:
		case fl8:
		case none:
			outFrame->dataVal.dataValueBU.dataByte1 =
				(uint8_t)pMV->Val.iVal;
			break;
		case s8:
			outFrame->dataVal.dataValueBS.dataByte1 =
				(int8_t)pMV->Val.iVal;
			break;
		case u16:
			outFrame->dataVal.dataValueU16 =
				(uint16_t)pMV->Val.iVal;
			break;
		case s16:
			outFrame->dataVal.dataValueS16 = (int16_t)pMV->Val.iVal;
			break;
		case f88:
			break;
		}

		/* 2nd byte goes */
		switch (pt->msgDataType2) {
		case u8:
		case fl8:
		case none:
			outFrame->dataVal.dataValueBU.dataByte2 =
				(uint8_t)(pMV->Val.iVal >> 8u);
			break;
		case s8:
			outFrame->dataVal.dataValueBS.dataByte2 =
				(int8_t)(pMV->Val.iVal / 256);
			break;
		}
	} else {
		outFrame->dataVal.dataValueF88 = FloatTof88(pMV->Val.fVal);
	}
fExit:
	return retVal;
}

/**
  * deserializes data from the opentherm frame (message)
  * @param pMV pointer to the target MV
  * @param pmsg pointer to the message
  * @return openThermResult_t serialization status
  */
openThermResult_t OPENTHERM_SaveToMV(tMV *const pMV, uint32_t *const pmsg)
{
	openThermResult_t retVal = OPENTHERM_ResOK;

	if ((pMV == NULL) || (pmsg == NULL)) {
		retVal = OPENTHERM_ResNullPtr;
		goto fExit;
	}

	const opentThermMsg_t *const pt = GetMessageTblEntry(pMV->LD_ID);
	/* is there such dataId ?*/
	if (pt == NULL) {
		retVal = OPENTHERM_ResNoMVorSPS;
		goto fExit;
	}

	pMV->TimeStamp = getTime();

	openThermFrame_t *const frame = (openThermFrame_t *)pmsg;

	/* decode payload */
	/* separate 2 byte data */
	float oldFVal = 0.0f;
	int32_t oldIVal = 0;
	switch (pt->msgDataType1) {
	case f88:
		oldFVal = pMV->Val.fVal;
		pMV->Val.fVal = F88ToFloat(frame->dataVal.dataValueF88);
		break;
	case s16:
		oldIVal = pMV->Val.iVal;
		pMV->Val.iVal = (int32_t)frame->dataVal.dataValueS16;
		break;
	case u8:
	case s8:
	case fl8:
	case u16:
		oldIVal = pMV->Val.iVal;
		pMV->Val.iVal = (int32_t)frame->dataVal.dataValueU16;
		break;
	default:
		retVal = OPENTHERM_ResBadArg;
		break;
	}

	if (retVal == OPENTHERM_ResOK) {
		/* is val changed ? */
		if (pMV->MV_type == floatMV) {
			float diff = pMV->Val.fVal - oldFVal;
			diff = (diff < 0.0f) ? -diff : diff;
			pMV->ValChanged =
				(diff > pMV->MV_Aperture.fVal) ? Yes : No;
		} else {
			int32_t diff = pMV->Val.iVal - oldIVal;
			diff = (diff < 0) ? -diff : diff;
			pMV->ValChanged =
				(diff > pMV->MV_Aperture.iVal) ? Yes : No;
		}

		/* check ranges */
		if (pMV->MV_type == floatMV) {
			if ((pMV->Val.fVal >= pMV->Lowest.fVal) &&
			    (pMV->Val.fVal <= pMV->Highest.fVal)) {
				pMV->Quality = QUALITY_VALIDITY_GOOD;
			} else {
				pMV->Quality = (QUALITY_VALIDITY_INVALID |
						QUALITY_DETAIL_OUT_OF_RANGE);
			}
		} else {
			if ((pMV->Val.iVal >= pMV->Lowest.iVal) &&
			    (pMV->Val.iVal <= pMV->Highest.iVal)) {
				pMV->Quality = QUALITY_VALIDITY_GOOD;
			} else {
				pMV->Quality = (QUALITY_VALIDITY_INVALID |
						QUALITY_DETAIL_OUT_OF_RANGE);
			}
		}
	}
fExit:
	return retVal;
}

/**
  * updates the MV  after writing data to the slave
  * @param pMV pointer to the target MV
  * @param pmsg pointer to the message
  */
void OPENTHERM_UpdateMV(tMV *const pMV, uint32_t *const pmsg)
{
	return;
}
