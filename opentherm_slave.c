/**
  ******************************************************************************
  * @file    opentherm_slave.c
  * @author  turchenkov@gmail.com
  * @version V0.0.1
  * @date    10-Nov-2018
  * @brief   This file contains opentherm slave protocol functionality
  ******************************************************************************
  */

#include "config.h"
#include <stddef.h>
#include <stdint.h>
#include "opentherm.h"

/*
	Control flow:
	1.	Get received message OPENTHERM_ReadSlave | OPENTHERM_WriteSlave
	2.	Check parity
	3.	De-serialize
	4.	Find and update/read the target MV
	5.	Generate OPENTHERN_ReadAck | OPENTHERN_WriteAck
	6.	Serialize
	7.	Send
*/

/* slave MV array */
static tMV MV_array[MV_ARRAY_LENGTH];
static tMV (*const ptMV_array)[MV_ARRAY_LENGTH] = &MV_array;

/**
 * @brief InitSlave
 * @return openThermResult_t
 */
openThermResult_t OPENTHERM_InitSlave()
{
	if (OPENTHERM_InitMV(ptMV_array, (size_t)MV_ARRAY_LENGTH) != 0) {
		return OPENTHERM_ResNoMVorSPS;
	} else {
		return OPENTHERM_ResOK;
	}
}

/**
  * processes the message from the master
  * @param msg is the message received
  * @param commFun is a callback communication function
  * @param GetMV is a cllback function which returns an MV by its id
  * @return openThermResult_t
  */
openThermResult_t OPENTHERM_SlaveRespond(uint32_t msg,
					 uint32_t (*commFun)(uint32_t),
					 tMV *(*GetMV)(uint8_t))
{
	openThermResult_t retVal = OPENTHERM_ResOK;
	/* check parity */
	if (CheckParity32(msg) != 0) {
		/* nothing to do */
		retVal = OPENTHERM_ResParityErr;
		goto fExit;
	}

	/* as slave we must respond only to:
	 * MsgType READ-DATA -> READ-ACK || DATA-INVALID || UNKNOWN-DATAID
	 * MsgType WRITE-DATA -> WRITE-ACK || DATA-INVALID || UNKNOWN-DATAID
	 */
	openThermFrame_t *p = (openThermFrame_t *)&msg;

	p->byte1.parity =0u;

	if (!((p->byte1.msgType == MSG_TYPE_READ_DATA) ||
	      (p->byte1.msgType == MSG_TYPE_WRITE_DATA))) {
		/* nothing to do */
		retVal = OPENTHERM_ResBadMsgType;
		goto fExit;
	}

	/* find the appropriate MV */
	tMV *pMV = GetMV(p->dataID);
	if (pMV != NULL) {
		if (p->byte1.msgType == MSG_TYPE_READ_DATA) {
			retVal = OPENTHERM_PutDataToMsg(pMV, &msg);
			if (retVal == OPENTHERM_ResDataInvalid) {
				p->byte1.msgType = MSG_TYPE_DATA_INVALID;
			} else {
				p->byte1.msgType = MSG_TYPE_READ_ACK;
			}
		} else {
			/* MSG_TYPE_WRITE_DATA */
			retVal = OPENTHERM_SaveToMV(pMV, &msg);
			if (retVal == OPENTHERM_ResDataInvalid) {
				p->byte1.msgType = MSG_TYPE_DATA_INVALID;
			} else {
				p->byte1.msgType = MSG_TYPE_WRITE_ACK;
			}
		}
	} else {
		retVal = OPENTHERM_ResBadMsgId;
		p->dataVal.dataValueU16 = 0u;
	}
	/* set correct parity bit */
	if (CheckParity32(msg) != 0) {
		p->byte1.parity = 1u;
	}
	commFun(msg);

fExit:
	return retVal;
}
