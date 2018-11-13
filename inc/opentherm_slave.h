/**
  ******************************************************************************
  * @file    opentherm_slave.h
  * @author  turchenkov@gmail.com
  * @version V0.0.1
  * @date    10-Nov-2018
  * @brief   This file contains opentherm slave protocol functionality headers
  ******************************************************************************
  */

#ifndef OPENTHERM_SLAVE_H
#define OPENTHERM_SLAVE_H

#include "opentherm_data_def.h"

#ifdef __cplusplus
extern "C" {
#endif

openThermResult_t OPENTHERM_InitSlave(void);

openThermResult_t OPENTHERM_SlaveRespond(uint32_t msg, uint32_t (*commFun)(uint32_t), tMV* (*GetMV)(uint8_t));

#ifdef __cplusplus
}
#endif

#endif // OPENTHERM_SLAVE_H
