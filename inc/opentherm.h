/**
  ******************************************************************************
  * @file    opentherm.h
  * @author  turchenkov@gmail.com
  * @version V0.0.1
  * @date    07-Nov-2018
  * @brief   This file contains opentherm protocol entities definitions
  ******************************************************************************
  */

#ifndef OPENTHERM_H
#define OPENTHERM_H

#include <stdint.h>
#include "opentherm_version.h"
#include "opentherm_daq_def.h"
#include "message_table.h"
#include "opentherm_data_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Functions prototypes
*/
int32_t OPENTHERM_InitMV(tMV (*const pMVArr)[], size_t MVArrLen);

openThermResult_t OPENTHERM_SaveToMV(tMV *const pMV, uint32_t *const pmsg);

void OPENTHERM_UpdateMV(tMV *const pMV, uint32_t *const pmsg);

openThermResult_t OPENTHERM_PutDataToMsg(tMV *const pMV, uint32_t *const pmsg);

int CheckParity32(uint32_t val);

float F88ToFloat(f88_t val);

f88_t FloatTof88(float val);

tTime getTime(void);


/*
 * Exported variables
 */

//extern tMV (*const ptMV_array)[MV_ARRAY_LENGTH]; /* constant ptr to the tMV array */

//extern tMV MV_array[MV_ARRAY_LENGTH];

extern const opentThermMsg_t (*const pMessageTbl)
	[MSG_TBL_LENGTH]; /* constant ptr to the message table array */

extern const opentThermMsg_t messagesTbl[MSG_TBL_LENGTH];

#ifdef __cplusplus
}
#endif


#endif // OPENTHERM_H
