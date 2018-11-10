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


#define MSG_TBL_LENGTH	54
#define MV_ARRAY_LENGTH	MSG_TBL_LENGTH

/* exported variables */

extern const opentThermMsg_t messagesTbl[MSG_TBL_LENGTH];
extern const opentThermMsg_t (*const pMessageTbl)[MSG_TBL_LENGTH];

/* exported functions */
const opentThermMsg_t *GetMessageTblEntry(const uint8_t msgId);

#endif /* MESSAGE_TABLE_H */
