/**
  ******************************************************************************
  * @file    opentherm_master.h
  * @author  turchenkov@gmail.com
  * @version V0.0.1
  * @date    10-Nov-2018
  * @brief   This file contains opentherm master protocol functionality headers
  ******************************************************************************
  */

#ifndef OPENTHERM_MASTER_H
#define OPENTHERM_MASTER_H

#include "opentherm_data_def.h"

openThermResult_t OPENTHERM_InitMaster(void);

openThermResult_t OPENTHERM_ReadSlave(tMV *const pMV, uint32_t (*commFun)(uint32_t));

openThermResult_t OPENTHERM_WriteSlave(tMV *const pMV, uint32_t (*commFun)(uint32_t));

#endif // OPENTHERM_MASTER_H
