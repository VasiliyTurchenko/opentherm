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
//#include "message_table.h"

/* global variables */

/* static functions prototypes */

void initFloatMVFields(tMV *p);
void initIntMVFields(tMV *p);
void initSomeCommonMVFields(tMV *p);
openThermResult_t updateMVQualAndChg(tMV *const pMV, union MV_Val oldVal);

/**
  * @brief initializes the MV array by the data from MessageTable
  * @param pmv the pointer to MV array
  * @param pMVArrLen lenthth of the array
  * @return -1 if error, 0 otherwize
  *
  */
int32_t OPENTHERM_InitMV(tMV (*const pMVArr)[], size_t MVArrLen)
{
	if ((pMVArr == NULL) || (MVArrLen == 0u)) {
		return -1;
	}

	size_t MVindex = 0u;
	for (size_t msgcount = 0u; msgcount < (size_t)MSG_TBL_LENGTH; msgcount++) {
		tMV *p = &(*pMVArr)[MVindex];
		p->LD_ID = messagesTbl[msgcount].msgId;

		initSomeCommonMVFields(p);

		if (messagesTbl[msgcount].msgDataType1 == f88) {
			initFloatMVFields(p);

			p->MV_Unit = messagesTbl[msgcount].MV_Unit1;
			p->Lowest.fVal = messagesTbl[msgcount].Lowest_MV1.fVal;
			p->Highest.fVal =
				messagesTbl[msgcount].Highest_MV1.fVal;
		} else {
			initIntMVFields(p);

			p->MV_Unit = messagesTbl[msgcount].MV_Unit1;
			p->Lowest.iVal = messagesTbl[msgcount].Lowest_MV1.iVal;
			p->Highest.iVal =
				messagesTbl[msgcount].Highest_MV1.iVal;
		}
		if (messagesTbl[msgcount].msgDataType2 != none) {
			// 2 MVs will be initialized
			MVindex++;
			if (MVindex == MVArrLen) {
				break;
			}
			// advance the pointer
			p = &(*pMVArr)[MVindex];

			p->LD_ID = messagesTbl[msgcount].msgId;
			initSomeCommonMVFields(p);

			if (messagesTbl[msgcount].msgDataType2 == f88) {
				initFloatMVFields(p);

				p->MV_Unit = messagesTbl[msgcount].MV_Unit2;
				p->Lowest.fVal =
					messagesTbl[msgcount].Lowest_MV2.fVal;
				p->Highest.fVal =
					messagesTbl[msgcount].Highest_MV2.fVal;
			} else {
				initIntMVFields(p);

				p->MV_Unit = messagesTbl[msgcount].MV_Unit2;
				p->Lowest.iVal =
					messagesTbl[msgcount].Lowest_MV2.iVal;
				p->Highest.iVal =
					messagesTbl[msgcount].Highest_MV2.iVal;
			}
		}
		MVindex++;
		if (MVindex == MVArrLen) {
			break;
		}
	}
	return 0;
}

/**
 * @brief initFloatMVfields
 * @param p pointer to the MV to be initialized
 */
void initFloatMVFields(tMV *p)
{
	p->MV_type = floatMV;
	p->Val.fVal = 0.0f;
	p->MV_Shift.fVal = 0.0f;
	p->MV_Scale.fVal = 1.0f;
	p->MV_Aperture.fVal = 0.0f;
}

/**
 * @brief initIntMVValues
 * @param p pointer to the MV to be initialized
 */
void initIntMVFields(tMV *p)
{
	p->MV_type = intMV;
	p->Val.iVal = 0;
	p->MV_Shift.iVal = 0;
	p->MV_Scale.iVal = 1;
	p->MV_Aperture.iVal = 0;
}

/**
 * @brief initSomeCommonMVFields
 * @param p pointer to the MV to be initialized
 */
void initSomeCommonMVFields(tMV *p)
{
	p->Ctrl = No;
	p->ValChanged = Yes;
	p->TimeStamp = getTime();
	/* QUALITY_VALIDITY_GOOD - not correct */
	p->Quality = QUALITY_VALIDITY_GOOD;
}

// stub function
tTime getTime(void)
{
	tTime retVal;
	struct timespec spec;
	clock_gettime(CLOCK_REALTIME, &spec);
	retVal.Seconds = (uint32_t)spec.tv_sec;
	retVal.mSeconds = (uint32_t)spec.tv_nsec / 1000000u;
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
// clang-format off
	if ((val.int_part & 0x80u) != 0u) {
		retVal = (float)( -(0x10000 - ( ((uint16_t)(val.int_part) << 8) + val.frac_part) ) / 256.0f );
	} else {
		retVal = (float)(             ( ((uint16_t)(val.int_part) << 8) + val.frac_part  ) / 256.0f );
	}
// clang-format on
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
	int32_t a = (int32_t)(val * 256.0f);
	retVal.int_part = (uint8_t)(a >> 8);
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
  * @param pMV pointer to the MV - in case of 2 MVs pMV must point to the first one
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

	/* define is one or two MVs are involved */
	uint8_t usedMVs = (pt->msgDataType2 == none) ? 1u : 2u;

	/* second MV --> second byte (word) */
	/* advance pointer  */
	tMV *const pMV2 = pMV + 1;
	if (usedMVs == 2u) {
		/* there is a possible pitfall !*/
		if (pMV->LD_ID != pMV2->LD_ID) {
			/* bad MV provided - not a pair */
			retVal = OPENTHERM_ResNoMVorSPS;
			goto fExit;
		}
	}
	/* first MV ---> first byte (word) */
	if (pMV->MV_type == intMV) {
		switch (pt->msgDataType1) {
		case u8:
		case fl8:
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
		default:
			retVal = OPENTHERM_ResBadArg;
			break;
		}
	} else {
		outFrame->dataVal.dataValueF88 = FloatTof88(pMV->Val.fVal);
	}
	if (pMV->Quality != QUALITY_VALIDITY_GOOD) {
		retVal = OPENTHERM_ResDataInvalid;
	}
	if (usedMVs == 2u) {
		if (pMV2->MV_type == intMV) {
			switch (pt->msgDataType2) {
			case u8:
			case fl8:
				outFrame->dataVal.dataValueBU.dataByte2 =
					(uint8_t)pMV2->Val.iVal;
				break;
			case s8:
				outFrame->dataVal.dataValueBS.dataByte2 =
					(int8_t)pMV2->Val.iVal;
				break;
			default:
				retVal = OPENTHERM_ResBadArg;
				break;
			}
		} else {
			/* no float 8.8 can be in the 2nd MV */
			retVal = OPENTHERM_ResBadArg;
		}
		if (pMV2->Quality != QUALITY_VALIDITY_GOOD) {
			retVal = OPENTHERM_ResDataInvalid;
		}
	}
fExit:
	return retVal;
}

/**
  * deserializes data from the opentherm frame (message)
  * @param pMV pointer to the target MV - in the case of 2MVs pMV must point at first of them
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
	tMV *pMV2;
	/* define is one or two MVs are involved */
	uint8_t usedMVs = (pt->msgDataType2 == none) ? 1u : 2u;
	if (usedMVs == 2u) {
		pMV2 = pMV + 1;
		/* there is a possible pitfall !*/
		if (pMV->LD_ID != pMV2->LD_ID) {
			/* bad MV provided - not a pair */
			retVal = OPENTHERM_ResNoMVorSPS;
			goto fExit;
		}
	}

	pMV->TimeStamp = getTime();

	openThermFrame_t *const frame = (openThermFrame_t *)pmsg;

	/* decode payload */
	union MV_Val oldVal;
	union MV_Val oldVal2;
	oldVal.iVal = 0;
	oldVal2.iVal = 0;

	if (pt->msgDataType1 == f88) {
		oldVal.fVal = pMV->Val.fVal;
		pMV->Val.fVal = F88ToFloat(frame->dataVal.dataValueF88);
	} else {
		oldVal.iVal = pMV->Val.iVal;

		switch (pt->msgDataType1) {
		case s16:
			pMV->Val.iVal = (int32_t)frame->dataVal.dataValueS16;
			break;
		case u16:
			pMV->Val.iVal = (int32_t)frame->dataVal.dataValueU16;
			break;
		case u8:
		case fl8:
			pMV->Val.iVal =
				(int32_t)frame->dataVal.dataValueBU.dataByte1;
			break;
		case s8:
			pMV->Val.iVal =
				(int32_t)frame->dataVal.dataValueBS.dataByte1;
			break;
		default:
			retVal = OPENTHERM_ResBadArg;
			break;
		}
	}
	/* second MV */
	if (usedMVs == 2u) {
		pMV2->TimeStamp = getTime();
		oldVal2.fVal = pMV->Val.fVal;
		switch (pt->msgDataType2) {
		case u8:
		case fl8:
			pMV2->Val.iVal =
				(int32_t)frame->dataVal.dataValueBU.dataByte2;
			break;
		case s8:
			pMV2->Val.iVal =
				(int32_t)frame->dataVal.dataValueBS.dataByte2;
			break;
		default:
			retVal = OPENTHERM_ResBadArg;
			break;
		}
	}

	if (retVal == OPENTHERM_ResOK) {
		/* is val changed ? */
		/* check ranges */
		retVal = updateMVQualAndChg(pMV, oldVal);

		if (usedMVs == 2u) {
			retVal = (updateMVQualAndChg(pMV2, oldVal2) ==
				  OPENTHERM_ResDataInvalid) ?
					 OPENTHERM_ResDataInvalid :
					 retVal;
		}
	}
fExit:
	return retVal;
}

/**
 * @brief updateMVQualAndChg
 * @param pMV pointer to MV to be updated
 * @param oldVal old value for comparison
 * @return OPENTHERM_ResDataInvalid | OPENTHERM_ResOk
 */
openThermResult_t updateMVQualAndChg(tMV *const pMV, union MV_Val oldVal)
{
	openThermResult_t retVal = OPENTHERM_ResOK;

	if (pMV->MV_type == floatMV) {
		float diff = pMV->Val.fVal - oldVal.fVal;
		diff = (diff < 0.0f) ? -diff : diff;
		pMV->ValChanged = (diff > pMV->MV_Aperture.fVal) ? Yes : No;
	} else {
		int32_t diff = pMV->Val.iVal - oldVal.iVal;
		diff = (diff < 0) ? -diff : diff;
		pMV->ValChanged = (diff > pMV->MV_Aperture.iVal) ? Yes : No;
	}
	if (pMV->MV_type == floatMV) {
		if ((pMV->Val.fVal >= pMV->Lowest.fVal) &&
		    (pMV->Val.fVal <= pMV->Highest.fVal)) {
			pMV->Quality = QUALITY_VALIDITY_GOOD;
		} else {
			pMV->Quality = (QUALITY_VALIDITY_INVALID |
					QUALITY_DETAIL_OUT_OF_RANGE);
			retVal = OPENTHERM_ResDataInvalid;
		}
	} else {
		if ((pMV->Val.iVal >= pMV->Lowest.iVal) &&
		    (pMV->Val.iVal <= pMV->Highest.iVal)) {
			pMV->Quality = QUALITY_VALIDITY_GOOD;
		} else {
			pMV->Quality = (QUALITY_VALIDITY_INVALID |
					QUALITY_DETAIL_OUT_OF_RANGE);
			retVal = OPENTHERM_ResDataInvalid;
		}
	}
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
