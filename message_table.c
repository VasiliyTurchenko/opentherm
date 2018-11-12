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


{MSG_ID_STATUS,		rd,	fl8,	fl8,	0u,	BitField,	BitField,	.Lowest_MV1.iVal = 0,	.Highest_MV1.iVal = 255,
											.Lowest_MV2.iVal = 0,	.Highest_MV2.iVal = 255},

{MSG_ID_TSET,		wr,	f88,	none,	0u,	Cels,		Unnamed,	.Lowest_MV1.fVal = 0.0f,.Highest_MV1.fVal = 100.0f},		// 1

{MSG_ID_M_CONFIG,	wr,	fl8,	u8,	2u,	BitField,	Unnamed,	.Lowest_MV1.iVal = 0,	.Highest_MV1.iVal = 255,
											.Lowest_MV2.iVal = 0,	.Highest_MV2.iVal = 255},

{MSG_ID_S_CONFIG,	rd,	fl8,	u8,	2u,	BitField,	Unnamed,	.Lowest_MV1.iVal = 0,	.Highest_MV1.iVal = 255,
											.Lowest_MV2.iVal = 0,	.Highest_MV2.iVal = 255},

{MSG_ID_COMMAND,	rw,	u8,	u8,	3u,	Unnamed,	Unnamed,	.Lowest_MV1.iVal = 0,	.Highest_MV1.iVal = 255,
											.Lowest_MV2.iVal = 0,	.Highest_MV2.iVal = 255},

{MSG_ID_ASF_FLAGS,	rd,	fl8,	u8,	0u,	BitField,	Unnamed,	.Lowest_MV1.iVal = 0,	.Highest_MV1.iVal = 255,
											.Lowest_MV2.iVal = 0,	.Highest_MV2.iVal = 255},

{MSG_ID_RBP_FLAGS,	rd,	fl8,	fl8,	5u,	BitField,	BitField,	.Lowest_MV1.iVal = 0,	.Highest_MV1.iVal = 255,
											.Lowest_MV2.iVal = 0,	.Highest_MV2.iVal = 255},

{MSG_ID_COOLING_CTRL,	wr,	f88,	none,	8u,	percent,	Unnamed,	.Lowest_MV1.fVal = 0.0f,.Highest_MV1.fVal = 100.0f},		// 2
{MSG_ID_TSETCH2,	wr,	f88,	none,	0u,	Cels,		Unnamed,	.Lowest_MV1.fVal = 0.0f,.Highest_MV1.fVal = 100.0f},		// 3
{MSG_ID_OVERRIDE,	rd,	f88,	none,	8u,	Cels,		Unnamed,	.Lowest_MV1.fVal = 0.0f,.Highest_MV1.fVal = 100.0f},		// 4

{MSG_ID_TSP,		rd,	u8,	u8,	6u,	Unnamed,	Unnamed,	.Lowest_MV1.iVal = 0,	.Highest_MV1.iVal = 255,
											.Lowest_MV2.iVal = 0,	.Highest_MV2.iVal = 255},

{MSG_ID_TSP_INDEX,	rw,	u8,	u8,	6u,	Unnamed,	Unnamed,	.Lowest_MV1.iVal = 0,	.Highest_MV1.iVal = 255,
											.Lowest_MV2.iVal = 0,	.Highest_MV2.iVal = 255},

{MSG_ID_FHB_SIZE,	rd,	u8,	u8,	7u,	Unnamed,	Unnamed,	.Lowest_MV1.iVal = 0,	.Highest_MV1.iVal = 255,
											.Lowest_MV2.iVal = 0,	.Highest_MV2.iVal = 255},

{MSG_ID_FHB_INDEX,	rd,	u8,	u8,	7u,	Unnamed,	Unnamed,	.Lowest_MV1.iVal = 0,	.Highest_MV1.iVal = 255,
											.Lowest_MV2.iVal = 0,	.Highest_MV2.iVal = 255},
{MSG_ID_MAX_REL_MOD_L_SET,
			wr,	f88,	none,	8u,	percent,	Unnamed,	.Lowest_MV1.fVal = 0.0f,.Highest_MV1.fVal = 100.0f},		// 5

{MSG_ID_MAX_CAPACITY,	rd,	u8,	u8,	8u,	kW,		percent,	.Lowest_MV1.iVal = 0,	.Highest_MV1.iVal = 255,
											.Lowest_MV2.iVal = 0,	.Highest_MV2.iVal = 100},

{MSG_ID_TRSET,		wr,	f88,	none,	4u,	Cels,		Unnamed,	.Lowest_MV1.fVal = -40.0f,.Highest_MV1.fVal = 127.0f},	// 6
{MSG_ID_REL_MOD_LEVEL,	rd,	f88,	none,	4u,	percent,	Unnamed,	.Lowest_MV1.fVal = 0.0f,.Highest_MV1.fVal = 100.0f},		// 7
{MSG_ID_CH_PRESSURE,	rd,	f88,	none,	4u,	bar,		Unnamed,	.Lowest_MV1.fVal=0.0f,	.Highest_MV1.fVal=5.0f},		// 8
{MSG_ID_DHW_FLOW_RATE,	rd,	f88,	none,	4u,	lpm,		Unnamed,	.Lowest_MV1.fVal=0.0f,	.Highest_MV1.fVal=16.0f},		// 9
{MSG_ID_DAY_TIME,	rw,	u8,	u8,	4u,	Unnamed,	Unnamed,	.Lowest_MV1.iVal = 0,	.Highest_MV1.iVal = 255,
											.Lowest_MV2.iVal = 0,	.Highest_MV2.iVal = 255},

{MSG_ID_DATE,		rw,	u8,	u8,	4u,	Unnamed,	Unnamed,	.Lowest_MV1.iVal = 0,	.Highest_MV1.iVal = 255,
											.Lowest_MV2.iVal = 0,	.Highest_MV2.iVal = 255},

{MSG_ID_YEAR,		rw,	u16,	none,	4u,	Unnamed,	Unnamed,	.Lowest_MV1.iVal=1900,	.Highest_MV1.iVal=2100},		// 10
{MSG_ID_TRSETCH2,	wr,	f88,	none,	4u,	Cels,		Unnamed,	.Lowest_MV1.fVal=-40.0f,.Highest_MV1.fVal=127.0f},		// 11
{MSG_ID_TR,		wr,	f88,	none,	4u,	Cels,		Unnamed,	.Lowest_MV1.fVal=-40.0f,.Highest_MV1.fVal=127.0f},		// 12
{MSG_ID_TBOILER,	rd,	f88,	none,	4u,	Cels,		Unnamed,	.Lowest_MV1.fVal=-40.0f,.Highest_MV1.fVal=127.0f},		// 13
{MSG_ID_TDHW,		rd,	f88,	none,	4u,	Cels,		Unnamed,	.Lowest_MV1.fVal=-40.0f,.Highest_MV1.fVal=127.0f},		// 14
{MSG_ID_TOUTSIDE,	rd,	f88,	none,	4u,	Cels,		Unnamed,	.Lowest_MV1.fVal=-40.0f,.Highest_MV1.fVal=127.0f},		// 15
{MSG_ID_TRET,		rd,	f88,	none,	4u,	Cels,		Unnamed,	.Lowest_MV1.fVal=-40.0f,.Highest_MV1.fVal=127.0f},		// 16
{MSG_ID_TSTORAGE,	rd,	f88,	none,	4u,	Cels,		Unnamed,	.Lowest_MV1.fVal=-40.0f,.Highest_MV1.fVal=127.0f},		// 17
{MSG_ID_TCOLLECTOR,	rd,	f88,	none,	4u,	Cels,		Unnamed,	.Lowest_MV1.fVal=-40.0f,.Highest_MV1.fVal=250.0f},		// 18
{MSG_ID_TFLOWCH2,	rd,	f88,	none,	4u,	Cels,		Unnamed,	.Lowest_MV1.fVal=-40.0f,.Highest_MV1.fVal=127.0f},		// 19
{MSG_ID_TDHW2,		rd,	f88,	none,	4u,	Cels,		Unnamed,	.Lowest_MV1.fVal=-40.0f,.Highest_MV1.fVal=127.0f},		// 20
{MSG_ID_TEXHAUST,	rd,	s16,	none,	4u,	Cels,		Unnamed,	.Lowest_MV1.iVal=-40,	.Highest_MV1.iVal=500},		// 21

{MSG_ID_TDHWSET_UB_LB,	rd,	s8,	s8,	5u,	Cels,		Cels,		.Lowest_MV1.iVal = 0,	.Highest_MV1.iVal = 127,
											.Lowest_MV2.iVal = 0,	.Highest_MV2.iVal = 127},

{MSG_ID_MAXTSET_UB_LB,	rd,	s8,	s8,	5u,	Cels,		Cels,		.Lowest_MV1.iVal = 0,	.Highest_MV1.iVal = 127,
											.Lowest_MV2.iVal = 0,	.Highest_MV2.iVal = 127},

{MSG_ID_HCRATIO_UB_LB,	rd,	s8,	s8,	5u,	Unnamed,	Unnamed,	.Lowest_MV1.iVal = -127,.Highest_MV1.iVal = 127,
											.Lowest_MV2.iVal = -127,.Highest_MV2.iVal = 127},

{MSG_ID_TDHWSET,	rw,	f88,	none,	5u,	Cels,		Unnamed,	.Lowest_MV1.fVal=0.0f,	.Highest_MV1.fVal=127.0f},		// 22
{MSG_ID_MAXTSET,	rw,	f88,	none,	5u,	Cels,		Unnamed,	.Lowest_MV1.fVal=0.0f,	.Highest_MV1.fVal=127.0f},		// 23
{MSG_ID_HCRATIO,	rw,	f88,	none,	5u,	Unnamed,	Unnamed,	.Lowest_MV1.fVal=0.0f,	.Highest_MV1.fVal=127.0f},		// 24

{MSG_ID_REMOTE_OVERRIDE,rd,	fl8,	none,	8u,	BitField,	Unnamed,	.Lowest_MV1.iVal = 0,	.Highest_MV1.iVal = 255},		// 25

{MSG_ID_OEM_DIAG_CODE,	rd,	u16,	none,	0u,	Unnamed,	Unnamed,	.Lowest_MV1.iVal = 0,	.Highest_MV1.iVal = 65536},		// 26
{MSG_ID_BURNER_STARTS,	rw,	u16,	none,	4u,	Unnamed,	Unnamed,	.Lowest_MV1.iVal = 0,	.Highest_MV1.iVal = 65536},		// 27
{MSG_ID_CH_PUMP_STARTS,	rw,	u16,	none,	4u,	Unnamed,	Unnamed,	.Lowest_MV1.iVal = 0,	.Highest_MV1.iVal = 65536},		// 28
{MSG_ID_DHW_PUMP_STARTS,rw,	u16,	none,	4u,	Unnamed,	Unnamed,	.Lowest_MV1.iVal = 0,	.Highest_MV1.iVal = 65536},		// 29
{MSG_ID_DHW_BURNER_STARTS,
			rw,	u16,	none,	4u,	Unnamed,	Unnamed,	.Lowest_MV1.iVal = 0,	.Highest_MV1.iVal = 65536},		// 30

{MSG_ID_BURNER_OP_HOURS,rw,	u16,	none,	4u,	Hours,		Unnamed,	.Lowest_MV1.iVal = 0,	.Highest_MV1.iVal = 65536},		// 31
{MSG_ID_CH_PUMP_OP_HOURS,
			rw,	u16,	none,	4u,	Hours,		Unnamed,	.Lowest_MV1.iVal = 0,	.Highest_MV1.iVal = 65536},		// 32
{MSG_ID_DHW_PUMP_OP_HOURS,
			rw,	u16,	none,	4u,	Hours,		Unnamed,	.Lowest_MV1.iVal = 0,	.Highest_MV1.iVal = 65536},		// 33
{MSG_ID_DHW_BURNER_OP_HOURS,
			rw,	u16,	none,	4u,	Hours,		Unnamed,	.Lowest_MV1.iVal = 0,	.Highest_MV1.iVal = 65536},		// 34

{MSG_ID_OTH_VER_MASTER,	wr,	f88,	none,	2u,	Unnamed,	Unnamed,	.Lowest_MV1.fVal=0.0f,	.Highest_MV1.fVal=127.0f},		// 35
{MSG_ID_OTH_VER_SLAVE,	rd,	f88,	none,	2u,	Unnamed,	Unnamed,	.Lowest_MV1.fVal=0.0f,	.Highest_MV1.fVal=127.0f},		// 36

{MSG_ID_MASTER_VER,	wr,	u8,	u8,	2u,	Unnamed,	Unnamed,	.Lowest_MV1.iVal = 0,	.Highest_MV1.iVal = 255,
											.Lowest_MV2.iVal = 0,	.Highest_MV2.iVal = 255},

{MSG_ID_SLAVE_VER,	rd,	u8,	u8,	2u,	Unnamed,	Unnamed,	.Lowest_MV1.iVal = 0,	.Highest_MV1.iVal = 255,
											.Lowest_MV2.iVal = 0,	.Highest_MV2.iVal = 255},
};
// clang-format on

/* static check size of the table vs MSG_TBL_LENGTH */
CASSERT( MSG_TBL_LENGTH == ( sizeof(messagesTbl) / sizeof(messagesTbl[0]) ) , message_table_c )

CASSERT( ( SINGLE_MV_MESSAGES + DOUBLE_MV_MESSAGES ) == ( sizeof(messagesTbl) / sizeof(messagesTbl[0]) ) , message_table_c )

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
