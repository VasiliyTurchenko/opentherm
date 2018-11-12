/**
  ******************************************************************************
  * @file    message_table.h
  * @author  turchenkov@gmail.com
  * @version V0.0.1
  * @date    09-Nov-2018
  * @brief   This file contains definition of the opentherm commands table
  ******************************************************************************
  */

#ifndef MESSAGE_TABLE_H
#define MESSAGE_TABLE_H

#include "opentherm_data_def.h"
#include "CAssert.h"


/* total number of the opentherm messages id */
#define MSG_TBL_LENGTH	(SINGLE_MV_MESSAGES + DOUBLE_MV_MESSAGES)

#define MV_ARRAY_LENGTH	(SINGLE_MV_MESSAGES + DOUBLE_MV_MESSAGES * 2)

/* exported variables */

extern const opentThermMsg_t messagesTbl[MSG_TBL_LENGTH];
extern const opentThermMsg_t (*const pMessageTbl)[MSG_TBL_LENGTH];

/* exported functions */
const opentThermMsg_t *GetMessageTblEntry(const uint8_t msgId);

#endif /* MESSAGE_TABLE_H */
