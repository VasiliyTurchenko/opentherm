/**
  ******************************************************************************
  * @file    message_table.c
  * @author  turchenkov@gmail.com
  * @version V0.0.1
  * @date    09-Nov-2018
  * @brief   This file contains definition of the opentherm commands table
  ******************************************************************************
  */

#include <stddef.h>
#include "message_table.h"

// clang-format off
const
opentThermMsg_t messagesTbl[MSG_TBL_LENGTH] = {
/*0*/	{.msgId = MSG_ID_STATUS,           rd, fl8, fl8,   0u, BitField, BitField, .Lowest.iVal = INT16_MIN,   .Highest.iVal = INT16_MAX},       /* u16 */
	{.msgId = MSG_ID_TSET,             wr, f88, f88,   0u, Cels,     Cels,     .Lowest.fVal = 0.0f,       .Highest.fVal = 100.0f},
	{.msgId = MSG_ID_M_CONFIG,         wr, fl8, u8,    2u, BitField, Unnamed,  .Lowest.iVal = INT16_MIN,   .Highest.iVal = INT16_MAX},        /* 2 variables */
	{.msgId = MSG_ID_S_CONFIG,         rd, fl8, u8,    2u, BitField, Unnamed,  .Lowest.iVal = INT16_MIN,   .Highest.iVal = INT16_MAX},        /* 2 variables */
	{.msgId = MSG_ID_COMMAND,          wr, u8,  u8,    3u, Unnamed,  Unnamed,  .Lowest.iVal = INT16_MIN,   .Highest.iVal = INT16_MAX},        /* 2 variables */
	{.msgId = MSG_ID_ASF_FLAGS,        rd, fl8, u8,    0u, BitField, Unnamed,  .Lowest.iVal = INT16_MIN,   .Highest.iVal = INT16_MAX},        /* 2 variables */
	{.msgId = MSG_ID_RBP_FLAGS,        rd, fl8, fl8,   5u, BitField, BitField, .Lowest.iVal = INT16_MIN,   .Highest.iVal = INT16_MAX},        /* 2 variables */
	{.msgId = MSG_ID_COOLING_CONTROL,  wr, f88, f88,   8u, percent,  percent,  .Lowest.fVal = 0.0f,       .Highest.fVal = 100.0f},

/*8*/	{.msgId = MSG_ID_TSETCH2,          wr, f88, f88,   0u, Cels,     Cels,     .Lowest.fVal = 0.0f,       .Highest.fVal = 100.0f},
	{.msgId = MSG_ID_OVERRIDE,         rd, f88, f88,   8u, Cels,     Cels,     .Lowest.fVal = 0.0f,       .Highest.fVal = 100.0f},
	{.msgId = MSG_ID_TSP,              rd, u8,  u8,    6u, Unnamed,  Unnamed,  .Lowest.iVal = INT16_MIN,   .Highest.iVal = INT16_MAX},        /* 2 variables */
	{.msgId = MSG_ID_TSP_INDEX,        rw, u8,  u8,    6u, Unnamed,  Unnamed,  .Lowest.iVal = INT16_MIN,   .Highest.iVal = INT16_MAX},        /* 2 variables */
	{.msgId = MSG_ID_FHB_SIZE,         rd, u8,  u8,    7u, Unnamed,  Unnamed,  .Lowest.iVal = INT16_MIN,   .Highest.iVal = INT16_MAX},        /* 2 variables */
	{.msgId = MSG_ID_FHB_INDEX,        rd, u8,  u8,    7u, Unnamed,  Unnamed,  .Lowest.iVal = INT16_MIN,   .Highest.iVal = INT16_MAX},        /* 2 variables */
	{.msgId = MSG_ID_MAX_REL_MOD_LEV_SETTING, wr, f88, f88, 8u, percent,  percent,   .Lowest.fVal = 0.0f, .Highest.fVal = 100.0f},
	{.msgId = MSG_ID_MAX_CAPACITY,     rd, u8,  u8,    8u, kW,       percent,  .Lowest.iVal = 0,          .Highest.iVal = 65536},        /* 2 variables */

/*16*/	{.msgId = MSG_ID_TRSET,            wr, f88, f88,   4u, Cels,     Cels,     .Lowest.fVal = -40.0f,     .Highest.fVal = 127.0f},
	{.msgId = MSG_ID_REL_MOD_LEVEL,    rd, f88, f88,   4u, percent,  percent,  .Lowest.fVal = 0.0f,       .Highest.fVal = 100.0f},
	{.msgId = MSG_ID_CH_PRESSURE,      rd, f88, f88,   4u, bar,      bar,      .Lowest.fVal = 0.0f,       .Highest.fVal = 5.0f},
	{.msgId = MSG_ID_DHW_FLOW_RATE,    rd, f88, f88,   4u, lpm,      lpm,      .Lowest.fVal = 0.0f,       .Highest.fVal = 16.0f},
	{.msgId = MSG_ID_DAY_TIME,         rw, u8,  u8,    4u, Unnamed,  Unnamed,  .Lowest.iVal = INT16_MIN,   .Highest.iVal = INT16_MAX},        /* 2 variables */
	{.msgId = MSG_ID_DATE,             rw, u8,  u8,    4u, Unnamed,  Unnamed,  .Lowest.iVal = INT16_MIN,   .Highest.iVal = INT16_MAX},        /* 2 variables */
	{.msgId = MSG_ID_YEAR,             rw, u16, u16,   4u, Unnamed,  Unnamed,  .Lowest.iVal = 1900,       .Highest.iVal = 2100},
	{.msgId = MSG_ID_TRSETCH2,         wr, f88, f88,   4u, Cels,     Cels,     .Lowest.fVal = -40.0f,     .Highest.fVal = 127.0f},

/*24*/	{.msgId = MSG_ID_TR,               wr, f88, f88,   4u, Cels,     Cels,     .Lowest.fVal = -40.0f,     .Highest.fVal = 127.0f},
	{.msgId = MSG_ID_TBOILER,          rd, f88, f88,   4u, Cels,     Cels,     .Lowest.fVal = -40.0f,     .Highest.fVal = 127.0f},
	{.msgId = MSG_ID_TDHW,             rd, f88, f88,   4u, Cels,     Cels,     .Lowest.fVal = -40.0f,     .Highest.fVal = 127.0f},
	{.msgId = MSG_ID_TOUTSIDE,         rd, f88, f88,   4u, Cels,     Cels,     .Lowest.fVal = -40.0f,     .Highest.fVal = 127.0f},
	{.msgId = MSG_ID_TRET,             rd, f88, f88,   4u, Cels,     Cels,     .Lowest.fVal = -40.0f,     .Highest.fVal = 127.0f},
	{.msgId = MSG_ID_TSTORAGE,         rd, f88, f88,   4u, Cels,     Cels,     .Lowest.fVal = -40.0f,     .Highest.fVal = 127.0f},
	{.msgId = MSG_ID_TCOLLECTOR,       rd, f88, f88,   4u, Cels,     Cels,     .Lowest.fVal = -40.0f,     .Highest.fVal = 250.0f},
	{.msgId = MSG_ID_TFLOWCH2,         rd, f88, f88,   4u, Cels,     Cels,     .Lowest.fVal = -40.0f,     .Highest.fVal = 127.0f},

/*32*/	{.msgId = MSG_ID_TDHW2,            rd, f88, f88,   4u, Cels,     Cels,     .Lowest.fVal = -40.0f,     .Highest.fVal = 127.0f},
	{.msgId = MSG_ID_TEXHAUST,         rd, s16, s16,   4u, Cels,     Cels,     .Lowest.iVal = -40,       .Highest.iVal = 500},
	{.msgId = MSG_ID_TDHWSET_UB_LB,    rd, s8,  s8,    5u, Cels,     Cels,     .Lowest.iVal = 0,         .Highest.iVal = 127},        /* 2 variables */
	{.msgId = MSG_ID_MAXTSET_UB_LB,    rd, s8,  s8,    5u, Cels,     Cels,     .Lowest.iVal = 0,         .Highest.iVal = 127},        /* 2 variables */
	{.msgId = MSG_ID_HCRATIO_UB_LB,    rd, s8,  s8,    5u, Unnamed,  Unnamed,  .Lowest.iVal = INT16_MIN,   .Highest.iVal = INT16_MAX},    /* 2 variables */
	{.msgId = MSG_ID_TDHWSET,          rw, f88, f88,   5u, Cels,     Cels,     .Lowest.fVal = 0.0f,       .Highest.fVal = 127.0f},
	{.msgId = MSG_ID_MAXTSET,          rw, f88, f88,   5u, Cels,     Cels,     .Lowest.fVal = 0.0f,       .Highest.fVal = 127.0f},
	{.msgId = MSG_ID_HCRATIO,          rw, f88, f88,   5u, Cels,     Cels,     .Lowest.fVal = 0.0f,       .Highest.fVal = 1000.0f},

/*40*/	{.msgId = MSG_ID_REMOTE_OVERRIDE,  rd, fl8, none,  8u, BitField, Unnamed,  .Lowest.iVal = INT16_MIN,   .Highest.iVal = INT16_MAX},
	{.msgId = MSG_ID_OEM_DIAG_CODE,    rd, u16, u16,   0u, Unnamed,  Unnamed,  .Lowest.iVal = INT16_MIN,   .Highest.iVal = INT16_MAX},
	{.msgId = MSG_ID_BURNER_STARTS,    rw, u16, u16,   4u, Unnamed,  Unnamed,  .Lowest.iVal = 0,         .Highest.iVal = 65536},
	{.msgId = MSG_ID_CH_PUMP_STARTS,   rw, u16, u16,   4u, Unnamed,  Unnamed,  .Lowest.iVal = 0,         .Highest.iVal = 65536},
	{.msgId = MSG_ID_DHW_PUMP_STARTS,  rw, u16, u16,   4u, Unnamed,  Unnamed,  .Lowest.iVal = 0,         .Highest.iVal = 65536},
	{.msgId = MSG_ID_DHW_BURNER_STARTS,rw, u16, u16,   4u, Unnamed,  Unnamed,  .Lowest.iVal = 0,         .Highest.iVal = 65536},
	{.msgId = MSG_ID_BURNER_OP_HOURS,  rw, u16, u16,   4u, Hours,    Hours,    .Lowest.iVal = 0,         .Highest.iVal = 65536},
	{.msgId = MSG_ID_CH_PUMP_OP_HOURS, rw, u16, u16,   4u, Hours,    Hours,    .Lowest.iVal = 0,         .Highest.iVal = 65536},

/*48*/	{.msgId = MSG_ID_DHW_PUMP_OP_HOURS,rw, u16, u16,   4u, Hours,    Hours,    .Lowest.iVal = 0,         .Highest.iVal = 65536},
	{.msgId = MSG_ID_DHW_BURNER_OP_HOURS,rw, u16, u16, 4u, Hours,    Hours,    .Lowest.iVal = 0,         .Highest.iVal = 65536},
	{.msgId = MSG_ID_OPENTHERM_VER_MASTER,wr, f88, f88,2u, Unnamed,  Unnamed,  .Lowest.fVal = 0.0f,       .Highest.fVal = 127.0f},
	{.msgId = MSG_ID_OPENTHERM_VER_SLAVE, rd, f88, f88,2u, Unnamed,  Unnamed,  .Lowest.fVal = 0.0f,       .Highest.fVal = 127.0f},
	{.msgId = MSG_ID_MASTER_VER,       wr, u8, u8,     2u, Unnamed,  Unnamed,  .Lowest.iVal = 0,         .Highest.iVal = 65536},        /* 2 variables */
	{.msgId = MSG_ID_SLAVE_VER,        rd, u8, u8,     2u, Unnamed,  Unnamed,  .Lowest.iVal = 0,         .Highest.iVal = 65536},        /* 2 variables */
};
// clang-format on

/* constant ptr to the tMV array */
const opentThermMsg_t (*const pMessageTbl)[MSG_TBL_LENGTH] = &messagesTbl;


/**
  * returns pointer to the requsested MessageTbl entry
  * @param msgId requested Id
  * @return NULL or pointer to the entry
  */

const opentThermMsg_t *GetMessageTblEntry(const uint8_t msgId)
{
	size_t i;
	for (i = 0u; i < MSG_TBL_LENGTH; i++) {
		if (msgId == messagesTbl[i].msgId) {
			return &(messagesTbl[i]);
		}
	}
	return NULL;
}
