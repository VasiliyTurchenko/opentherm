/**
  ******************************************************************************
  * @file    opentherm_json.h
  * @author  turchenkov@gmail.com
  * @version V1.0.0
  * @date    07-Nov-2018
  * @brief   This file contains opentherm to json definitions
  ******************************************************************************
  */

#ifndef OPENTHERM_JSON_H
#define OPENTHERM_JSON_H

#include <stdint.h>
#include "opentherm_daq_def.h"

uint8_t * ConvertMVToJSON(tMV *pMV);

#endif // OPENTHERM_JSON_H
