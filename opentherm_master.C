/**
  ******************************************************************************
  * @file    opentherm_master.c
  * @author  turchenkov@gmail.com
  * @version V0.0.1
  * @date    10-Nov-2018
  * @brief   This file contains opentherm master protocol functionality
  ******************************************************************************
  */

#include "config.h"
#include <stddef.h>
#include <stdint.h>
#include "opentherm.h"
//#include "message_table.h"

/*
	Control flow:
	1.	Get command OPENTHERM_ReadSlave | OPENTHERM_WriteSlave
	2.	Find and check the target MV
	3.	Serialize
	4.	Send

	5.	Wait for reply

	6.	Get reply
	7.	Check parity
	8.	De-serialize
	9.	OPENTHERM_ReadSlave -> OPENTHERN_ReadAck
		Save arrived data to the MV
			OR
		OPENTHERM_WriteSlave -> OPENTHERN_WriteAck
		Update the MV if needed
			OR
		process message with error
*/

static tMV MV_array[MV_ARRAY_LENGTH];

static tMV (*const ptMV_array)[MV_ARRAY_LENGTH] = &MV_array;

/**
 * @brief InitMaster
 * @return openThermResult_t
 */
openThermResult_t OPENTHERM_InitMaster() {
	if (OPENTHERM_InitMV(ptMV_array, (size_t)MV_ARRAY_LENGTH) != 0) {
		return OPENTHERM_ResNoMVorSPS;
	} else {
		return OPENTHERM_ResOK;
	}
}

/**
  * reads the slave
  * @param pMV the pointer to the MV
  * @param commFun is a callback communication function
  * @return openThermResult_t
  */
openThermResult_t OPENTHERM_ReadSlave(tMV *const pMV,
				      uint32_t (*commFun)(uint32_t))
{
	openThermResult_t retVal = OPENTHERM_ResOK;

	const opentThermMsg_t *const pt = GetMessageTblEntry(pMV->LD_ID);

	/* is there such dataId ?*/
	if (pt == NULL) {
		retVal = OPENTHERM_ResNoMVorSPS;
		goto fExit;
	}

	/* can we read this dataId ?*/
	if ((pt->msgMode != rd) && (pt->msgMode != rw)) {
		retVal = OPENTHERM_ResBadMsgId;
		goto fExit;
	}

	/* the frame */
	openThermFrame_t frame;
	frame.byte1.msgType = MSG_TYPE_READ_DATA;
	frame.dataID = pt->msgId;

	/* recast */
	uint32_t *pFrame;
	pFrame = (uint32_t *)&frame;
	if (CheckParity32(*pFrame) != 0) {
		frame.byte1.parity = 1u;
	}

	/* ready to communicate */
	uint32_t reply = commFun(*pFrame); /* ...... communication .......*/

	/* a reply arrived */
	if (CheckParity32(reply) != 0) {
		retVal = OPENTHERM_ResParityErr;
		goto fExit;
	}
	openThermFrame_t *pReplyFrame;
	/* recast */
	pReplyFrame = (openThermFrame_t *)&reply;

	if (pReplyFrame->byte1.msgType != MSG_TYPE_READ_ACK) {
		/* requested data is not read */
		if (pReplyFrame->byte1.msgType == MSG_TYPE_DATA_INVALID) {
			retVal = OPENTHERM_ResDataInvalid;
			goto fExit;
		}
		if (pReplyFrame->byte1.msgType == MSG_TYPE_UNKN_DATAID) {
			retVal = OPENTHERM_ResBadMsgId;
			goto fExit;
		}
		retVal = OPENTHERM_ResBadMsgType;
		goto fExit;
	}
	/* check if reply's dataId != requested dataId */
	if (pReplyFrame->dataID != frame.dataID) {
		retVal = OPENTHERM_ResBadMsgId;
		goto fExit;
	}
	/* read ack received, dataId is correct */
	retVal = OPENTHERM_SaveToMV(pMV, &reply);

fExit:
	return retVal;
}

/**
  * werites to the slave
  * @param pMV the pointer to the MV
  * @param commFun is a callback communication function
  * @return openThermResult_t
  */
openThermResult_t OPENTHERM_WriteSlave(tMV *const pMV,
				       uint32_t (*commFun)(uint32_t))
{
	openThermResult_t retVal = OPENTHERM_ResOK;

	const opentThermMsg_t *const pt = GetMessageTblEntry(pMV->LD_ID);

	/* is there such dataId ?*/
	if (pt == NULL) {
		retVal = OPENTHERM_ResNoMVorSPS;
		goto fExit;
	}

	/* can we read this dataId ?*/
	if ((pt->msgMode != rw) && (pt->msgMode != wr)) {
		retVal = OPENTHERM_ResBadMsgId;
		goto fExit;
	}

	/* the frame */
	openThermFrame_t frame;
	frame.byte1.msgType = MSG_TYPE_WRITE_DATA;
	frame.dataID = pt->msgId;

	/* recast */
	uint32_t *pFrame;
	pFrame = (uint32_t *)&frame;

	/* put data from MV to the frame */
	openThermResult_t putRes = OPENTHERM_PutDataToMsg(pMV, pFrame);
	if (putRes != OPENTHERM_ResOK) {
		retVal = putRes;
		goto fExit;
	}

	if (CheckParity32(*pFrame) != 0) {
		frame.byte1.parity = 1u;
	}

	/* ready to communicate */
	uint32_t reply = commFun(*pFrame); /* ...... communication .......*/

	/* a reply arrived */
	if (CheckParity32(reply) != 0) {
		retVal = OPENTHERM_ResParityErr;
		goto fExit;
	}
	openThermFrame_t *pReplyFrame;
	/* recast */
	pReplyFrame = (openThermFrame_t *)&reply;

	if (pReplyFrame->byte1.msgType != MSG_TYPE_WRITE_ACK) {
		/* requested data is not read */
		if (pReplyFrame->byte1.msgType == MSG_TYPE_DATA_INVALID) {
			retVal = OPENTHERM_ResDataInvalid;
			goto fExit;
		}
		if (pReplyFrame->byte1.msgType == MSG_TYPE_UNKN_DATAID) {
			retVal = OPENTHERM_ResBadMsgId;
			goto fExit;
		}
		retVal = OPENTHERM_ResBadMsgType;
		goto fExit;
	}
	/* check if reply's dataId != requested dataId */
	if (pReplyFrame->dataID != frame.dataID) {
		retVal = OPENTHERM_ResBadMsgId;
		goto fExit;
	}
	/* write ack received, dataId is correct */
	OPENTHERM_UpdateMV(pMV, &reply);

fExit:
	return retVal;
}
