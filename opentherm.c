/**
  ******************************************************************************
  * @file    opentherm.c
  * @author  turchenkov@gmail.com
  * @version V0.0.1
  * @date    07-Nov-2018
  * @brief   This file contains opentherm protocol functions
  ******************************************************************************
  */

#include    "config.h"
#include    <stddef.h>
#include    "opentherm.h"

/*
 * Messages table
 */
static
const
opentThermMsg_t messagesTbl[] = {
 {.msgId = MSG_ID_STATUS,           rd, fl8, fl8,   0u, BitField, BitField, .Lowest.iVal = INT_MIN,   .Highest.iVal = INT_MAX},       /* u16 */
 {.msgId = MSG_ID_TSET,             wr, f88, f88,   0u, Cels,     Cels,     .Lowest.fVal = 0.0f,       .Highest.fVal = 100.0f},
 {.msgId = MSG_ID_M_CONFIG,         wr, fl8, u8,    2u, BitField, Unnamed,  .Lowest.iVal = INT_MIN,   .Highest.iVal = INT_MAX},        /* 2 variables */
 {.msgId = MSG_ID_S_CONFIG,         rd, fl8, u8,    2u, BitField, Unnamed,  .Lowest.iVal = INT_MIN,   .Highest.iVal = INT_MAX},        /* 2 variables */
 {.msgId = MSG_ID_COMMAND,          wr, u8,  u8,    3u, Unnamed,  Unnamed,  .Lowest.iVal = INT_MIN,   .Highest.iVal = INT_MAX},        /* 2 variables */
 {.msgId = MSG_ID_ASF_FLAGS,        rd, fl8, u8,    0u, BitField, Unnamed,  .Lowest.iVal = INT_MIN,   .Highest.iVal = INT_MAX},        /* 2 variables */
 {.msgId = MSG_ID_RBP_FLAGS,        rd, fl8, fl8,   5u, BitField, BitField, .Lowest.iVal = INT_MIN,   .Highest.iVal = INT_MAX},        /* 2 variables */
 {.msgId = MSG_ID_COOLING_CONTROL,  wr, f88, f88,   8u, percent,  percent,  .Lowest.fVal = 0.0f,       .Highest.fVal = 100.0f},
 {.msgId = MSG_ID_TSETCH2,          wr, f88, f88,   0u, Cels,     Cels,     .Lowest.fVal = 0.0f,       .Highest.fVal = 100.0f},
 {.msgId = MSG_ID_OVERRIDE,         rd, f88, f88,   8u, Cels,     Cels,     .Lowest.fVal = 0.0f,       .Highest.fVal = 100.0f},

 {.msgId = MSG_ID_TSP,              rd, u8,  u8,    6u, Unnamed,  Unnamed,  .Lowest.iVal = INT_MIN,   .Highest.iVal = INT_MAX},        /* 2 variables */
 {.msgId = MSG_ID_TSP_INDEX,        rw, u8,  u8,    6u, Unnamed,  Unnamed,  .Lowest.iVal = INT_MIN,   .Highest.iVal = INT_MAX},        /* 2 variables */
 {.msgId = MSG_ID_FHB_SIZE,         rd, u8,  u8,    7u, Unnamed,  Unnamed,  .Lowest.iVal = INT_MIN,   .Highest.iVal = INT_MAX},        /* 2 variables */
 {.msgId = MSG_ID_FHB_INDEX,        rd, u8,  u8,    7u, Unnamed,  Unnamed,  .Lowest.iVal = INT_MIN,   .Highest.iVal = INT_MAX},        /* 2 variables */
 {.msgId = MSG_ID_MAX_REL_MOD_LEV_SETTING, wr, f88, f88, 8u, percent,  percent,   .Lowest.fVal = 0.0f, .Highest.fVal = 100.0f},
 {.msgId = MSG_ID_MAX_CAPACITY,     rd, u8,  u8,    8u, kW,       percent,  .Lowest.iVal = 0,          .Highest.iVal = 65536},        /* 2 variables */
 {.msgId = MSG_ID_TRSET,            wr, f88, f88,   4u, Cels,     Cels,     .Lowest.fVal = -40.0f,     .Highest.fVal = 127.0f},
 {.msgId = MSG_ID_REL_MOD_LEVEL,    rd, f88, f88,   4u, percent,  percent,  .Lowest.fVal = 0.0f,       .Highest.fVal = 100.0f},
 {.msgId = MSG_ID_CH_PRESSURE,      rd, f88, f88,   4u, bar,      bar,      .Lowest.fVal = 0.0f,       .Highest.fVal = 5.0f},
 {.msgId = MSG_ID_DHW_FLOW_RATE,    rd, f88, f88,   4u, lpm,      lpm,      .Lowest.fVal = 0.0f,       .Highest.fVal = 16.0f},

 {.msgId = MSG_ID_DAY_TIME,         rw, u8,  u8,    4u, Unnamed,  Unnamed,  .Lowest.iVal = INT_MIN,   .Highest.iVal = INT_MAX},        /* 2 variables */
 {.msgId = MSG_ID_DATE,             rw, u8,  u8,    4u, Unnamed,  Unnamed,  .Lowest.iVal = INT_MIN,   .Highest.iVal = INT_MAX},        /* 2 variables */
 {.msgId = MSG_ID_YEAR,             rw, u16, u16,   4u, Unnamed,  Unnamed,  .Lowest.iVal = 1900,       .Highest.iVal = 2100},
 {.msgId = MSG_ID_TRSETCH2,         wr, f88, f88,   4u, Cels,     Cels,     .Lowest.fVal = -40.0f,     .Highest.fVal = 127.0f},
 {.msgId = MSG_ID_TR,               wr, f88, f88,   4u, Cels,     Cels,     .Lowest.fVal = -40.0f,     .Highest.fVal = 127.0f},
 {.msgId = MSG_ID_TBOILER,          rd, f88, f88,   4u, Cels,     Cels,     .Lowest.fVal = -40.0f,     .Highest.fVal = 127.0f},
 {.msgId = MSG_ID_TDHW,             rd, f88, f88,   4u, Cels,     Cels,     .Lowest.fVal = -40.0f,     .Highest.fVal = 127.0f},
 {.msgId = MSG_ID_TOUTSIDE,         rd, f88, f88,   4u, Cels,     Cels,     .Lowest.fVal = -40.0f,     .Highest.fVal = 127.0f},
 {.msgId = MSG_ID_TRET,             rd, f88, f88,   4u, Cels,     Cels,     .Lowest.fVal = -40.0f,     .Highest.fVal = 127.0f},
 {.msgId = MSG_ID_TSTORAGE,         rd, f88, f88,   4u, Cels,     Cels,     .Lowest.fVal = -40.0f,     .Highest.fVal = 127.0f},

 {.msgId = MSG_ID_TCOLLECTOR,       rd, f88, f88,   4u, Cels,     Cels,     .Lowest.fVal = -40.0f,     .Highest.fVal = 250.0f},
 {.msgId = MSG_ID_TFLOWCH2,         rd, f88, f88,   4u, Cels,     Cels,     .Lowest.fVal = -40.0f,     .Highest.fVal = 127.0f},
 {.msgId = MSG_ID_TDHW2,            rd, f88, f88,   4u, Cels,     Cels,     .Lowest.fVal = -40.0f,     .Highest.fVal = 127.0f},
 {.msgId = MSG_ID_TEXHAUST,         rd, s16, s16,   4u, Cels,     Cels,     .Lowest.iVal = -40,       .Highest.iVal = 500},
 {.msgId = MSG_ID_TDHWSET_UB_LB,    rd, s8,  s8,    5u, Cels,     Cels,     .Lowest.iVal = 0,         .Highest.iVal = 127},        /* 2 variables */
 {.msgId = MSG_ID_MAXTSET_UB_LB,    rd, s8,  s8,    5u, Cels,     Cels,     .Lowest.iVal = 0,         .Highest.iVal = 127},        /* 2 variables */
 {.msgId = MSG_ID_HCRATIO_UB_LB,    rd, s8,  s8,    5u, Unnamed,  Unnamed,  .Lowest.iVal = INT_MIN,   .Highest.iVal = INT_MAX},    /* 2 variables */
 {.msgId = MSG_ID_TDHWSET,          rw, f88, f88,   5u, Cels,     Cels,     .Lowest.fVal = 0.0f,       .Highest.fVal = 127.0f},
 {.msgId = MSG_ID_MAXTSET,          rw, f88, f88,   5u, Cels,     Cels,     .Lowest.fVal = 0.0f,       .Highest.fVal = 127.0f},
 {.msgId = MSG_ID_HCRATIO,          rw, f88, f88,   5u, Cels,     Cels,     .Lowest.fVal = 0.0f,       .Highest.fVal = 1000.0f},

 {.msgId = MSG_ID_REMOTE_OVERRIDE,  rd, fl8, none,  8u, BitField, Unnamed,  .Lowest.iVal = INT_MIN,   .Highest.iVal = INT_MAX},
 {.msgId = MSG_ID_OEM_DIAG_CODE,    rd, u16, u16,   0u, Unnamed,  Unnamed,  .Lowest.iVal = INT_MIN,   .Highest.iVal = INT_MAX},
 {.msgId = MSG_ID_BURNER_STARTS,    rw, u16, u16,   4u, Unnamed,  Unnamed,  .Lowest.iVal = 0,         .Highest.iVal = 65536},
 {.msgId = MSG_ID_CH_PUMP_STARTS,   rw, u16, u16,   4u, Unnamed,  Unnamed,  .Lowest.iVal = 0,         .Highest.iVal = 65536},
 {.msgId = MSG_ID_DHW_PUMP_STARTS,  rw, u16, u16,   4u, Unnamed,  Unnamed,  .Lowest.iVal = 0,         .Highest.iVal = 65536},
 {.msgId = MSG_ID_DHW_BURNER_STARTS,rw, u16, u16,   4u, Unnamed,  Unnamed,  .Lowest.iVal = 0,         .Highest.iVal = 65536},
 {.msgId = MSG_ID_BURNER_OP_HOURS,  rw, u16, u16,   4u, Hours,    Hours,    .Lowest.iVal = 0,         .Highest.iVal = 65536},
 {.msgId = MSG_ID_CH_PUMP_OP_HOURS, rw, u16, u16,   4u, Hours,    Hours,    .Lowest.iVal = 0,         .Highest.iVal = 65536},
 {.msgId = MSG_ID_DHW_PUMP_OP_HOURS,rw, u16, u16,   4u, Hours,    Hours,    .Lowest.iVal = 0,         .Highest.iVal = 65536},
 {.msgId = MSG_ID_DHW_BURNER_OP_HOURS,rw, u16, u16, 4u, Hours,    Hours,    .Lowest.iVal = 0,         .Highest.iVal = 65536},

 {.msgId = MSG_ID_OPENTHERM_VER_MASTER,wr, f88, f88,2u, Unnamed,  Unnamed,  .Lowest.fVal = 0.0f,       .Highest.fVal = 127.0f},
 {.msgId = MSG_ID_OPENTHERM_VER_SLAVE, rd, f88, f88,2u, Unnamed,  Unnamed,  .Lowest.fVal = 0.0f,       .Highest.fVal = 127.0f},
 {.msgId = MSG_ID_MASTER_VER,       wr, u8, u8,     2u, Unnamed,  Unnamed,  .Lowest.iVal = 0,         .Highest.iVal = 65536},        /* 2 variables */
 {.msgId = MSG_ID_SLAVE_VER,        rd, u8, u8,     2u, Unnamed,  Unnamed,  .Lowest.iVal = 0,         .Highest.iVal = 65536},        /* 2 variables */
};

#define MSG_TBL_LENGTH ( (uint8_t)(sizeof(messagesTbl) / sizeof(messagesTbl[0])) )

const uint8_t MsgTblLength = MSG_TBL_LENGTH;

/* static functions prototypes */
void decodeFlags(flag8_t fl);
//float f88ToFloat(f88_t data);
int checkParity32(uint32_t val);

tTime getTime(void);

/* global variables */
tMV	MV_array[MSG_TBL_LENGTH];


/**
  * initializes the MV array
  * @param pmv the pointer to MV array
  * @param pMVArrLen lenthth of the array
  * @return -1 if error, 0 otherwize
  *
  */
int32_t OPENTHERM_InitMV(tMV (* const pMVArr)[], size_t MVArrLen) {

    if ((pMVArr == NULL) || (MVArrLen == 0)) {
        return -1;
    }

    if (MVArrLen > MsgTblLength ) {
        MVArrLen = MsgTblLength;
    }

    for (size_t i = 0; i < MVArrLen; i++) {
        // initialize only first MsgTblLength MVs
        tMV * p = &(*pMVArr)[i];
        p->LD_ID = messagesTbl[i].msgId;
        p->Ctrl = No;
        p->ValChanged = Yes;
        p->TimeStamp = getTime();
        p->Quality = QUALITY_VALIDITY_QUESTIONABLE;
        if (messagesTbl[i].msgDataType1 == f88) {
            p->MV_type = floatMV;
            p->Val.fVal = 0.0;
            p->MV_Unit = messagesTbl[i].MV_Unit1;
            p->MV_Shift.fVal = 0.0f;
            p->MV_Scale.fVal = 1.0f;
            p->MV_Aperture.fVal = 0.0f;
            p->Lowest.fVal = messagesTbl[i].Lowest.fVal;
            p->Highest.fVal = messagesTbl[i].Highest.fVal;
        } else {
            p->MV_type = intMV;
            p->Val.iVal = 0;
            p->MV_Unit = messagesTbl[i].MV_Unit1;
            p->MV_Shift.iVal = 0;
            p->MV_Scale.iVal = 1;
            p->MV_Aperture.iVal = 0;
            p->Lowest.iVal = messagesTbl[i].Lowest.iVal;
            p->Highest.iVal = messagesTbl[i].Highest.iVal;
        }
    }
    return 0;
}


/**
  * deserializes an receved opentherm frame
  * @param msg the frame as uint32 value
  * @param pmv the pointer to MV array
  * @param pMVArrLen lenthth of the array
  * @return openThermResult_t deserialization ststus
  *
  */
openThermResult_t OPENTHERM_DeserializeMsg(uint32_t msg, tMV (* const pMVArr)[], const size_t MVArrLen) {

    if ((pMVArr == NULL) || (MVArrLen == 0)) {
        return resBadArg;
    }

    openThermResult_t   retVal = resOK;
    const openThermFrame_p frame = (const openThermFrame_p)&msg;

    /* check parity */
    if (checkParity32(msg) != 0) {
        return  resParityErr;
    }

    /* check if answer correct */
    if (frame->byte1.msgType != MSG_TYPE_READ_ACK) {
        return resBadMsgType;
    }

    /* check if message has correct Id*/
    size_t  i;
    for (i = 0; i < MsgTblLength; i++) {
        if (frame->dataID == messagesTbl[i].msgId) { break; }
    }
#ifdef DESKTOP_TEST
    if ( i == MsgTblLength ) {
        return  resBadMsgId;         /* bad msgId */
    }
#else
    if ( (i == MsgTblLength) || !((messagesTbl[i].msgMode == rd) || (messagesTbl[i].msgMode == rw)) ) {
        return  resBadMsgId;         /* bad msgId */
    }
#endif

    /* check if corresponding MV is not exist*/
    size_t jj;
    for (jj = 0; jj < MVArrLen; jj++) {
        if (messagesTbl[i].msgId == (*pMVArr)[jj].LD_ID) {break; }
    }
    if (jj == MVArrLen) {
        return resNoMVorSPS;
    }

    /* fix pointer to the MV */
    tMV * p = &(*pMVArr)[jj];

    p->TimeStamp = getTime();

    /* decode payload */
    /* separate 2 byte data */
    float     oldFVal = 0.0f;
    int32_t    oldIVal = 0;
    switch (messagesTbl[i].msgDataType1) {
        case f88:
            oldFVal = p->Val.fVal;
            p->Val.fVal = f88ToFloat(frame->dataVal.dataValueF88);
            break;
        case s16:
            oldIVal = p->Val.iVal;
            p->Val.iVal = (int32_t)frame->dataVal.dataValueS16;
            break;
        case u8:
        case s8:
        case fl8:
        case u16:
            oldIVal = p->Val.iVal;
            p->Val.iVal = (int32_t)frame->dataVal.dataValueU16;
            break;
        default:
        return resBadMsgId;
    }

    /* is val changed ? */
    if (p->MV_type == floatMV) {
        float   diff = p->Val.fVal - oldFVal;
        diff = ( diff < 0.0f ) ? -diff : diff;
        p->ValChanged = (diff > p->MV_Aperture.fVal) ? Yes : No;
    } else {
        int32_t diff = p->Val.iVal - oldIVal;
        diff = ( diff < 0 ) ? -diff : diff;
        p->ValChanged = (diff > p->MV_Aperture.iVal) ? Yes : No;
    }

    /* check ranges */
    if (p->MV_type == floatMV) {
        if ( (p->Val.fVal >= p->Lowest.fVal) && (p->Val.fVal <= p->Highest.fVal) ) {
            p->Quality = QUALITY_VALIDITY_GOOD;
        } else {
            p->Quality = (QUALITY_VALIDITY_INVALID | QUALITY_DETAIL_OUT_OF_RANGE);
        }
    } else {
        if ( (p->Val.iVal >= p->Lowest.iVal) && (p->Val.iVal <= p->Highest.iVal) ) {
            p->Quality = QUALITY_VALIDITY_GOOD;
        } else {
            p->Quality = (QUALITY_VALIDITY_INVALID | QUALITY_DETAIL_OUT_OF_RANGE);
        }
    }

    return retVal;
}

/**
  * deserializes an receved opentherm frame
  * @param msg the frame as uint32 value
  * @param pmv the pointer to MV array
  * @param pMVArrLen lenthth of the array
  * @return openThermResult_t deserialization ststus
  *
  */
openThermResult_t OPENTHERM_SerializeMsg(uint32_t * const pmsg, tMV * const pMV, uint8_t msgType) {

    if ( (pmsg == NULL) || (pMV == NULL) || (msgType > MSG_TYPE_UNKN_DATAID) ) {
        return resBadArg;
    }
    openThermFrame_t outFrame;
//    uint32_t *p32 = (uint32_t*)&outFrame;
    *(uint32_t*)&outFrame = 0u;
//    *p32 = 0u;

    outFrame.byte1.msgType = msgType;

    /* find the data_id props in the table */
    size_t i;
    for (i = 0; i < MsgTblLength; i++) {
        if (pMV->LD_ID == messagesTbl[i].msgId) { break; }
    }
    if ( i == MsgTblLength ) {
        return  resBadMsgId;         /* bad msgId */
    }

    outFrame.dataID = pMV->LD_ID;
    if (pMV->MV_type == intMV) {
        switch (messagesTbl[i].msgDataType1) {
        case u8:
        case fl8:
        case none:
            outFrame.dataVal.dataValueBU.dataByte1 = (uint8_t)pMV->Val.iVal;
            break;
        case s8:
            outFrame.dataVal.dataValueBS.dataByte1 = (int8_t)pMV->Val.iVal;
            break;
        case u16:
            outFrame.dataVal.dataValueU16 = (uint16_t)pMV->Val.iVal;
            break;
        case s16:
            outFrame.dataVal.dataValueS16 = (int16_t)pMV->Val.iVal;
            break;
        case f88:
            break;
        }

        /* 2nd byte goes */
        switch (messagesTbl[i].msgDataType2) {
        case u8:
        case fl8:
        case none:
            outFrame.dataVal.dataValueBU.dataByte2 = (uint8_t)(pMV->Val.iVal >> 8u);
            break;
        case s8:
            outFrame.dataVal.dataValueBS.dataByte2 = (int8_t)(pMV->Val.iVal / 256);
            break;
        }
    } else {
        outFrame.dataVal.dataValueF88 = FloatTof88(pMV->Val.fVal);
    }
    *pmsg = *(uint32_t*)&outFrame;
    // check parity
    if (checkParity32(*pmsg) != 0 ) {
        outFrame.byte1.parity = 1u;
        *pmsg = *(uint32_t*)&outFrame;
    }

    openThermResult_t   retVal = resOK;
    return  retVal;
}


// stub function
tTime getTime(void) {
    tTime retVal = {1000, 989};
    return retVal;
}

/**
  * converts f88_t value to float
  * @param val input value
  * @return float value
  */
float f88ToFloat(f88_t val) {
    float retVal;
    if (val.int_part & 0x80u) {
        retVal = -(0x10000 - (((uint16_t)val.int_part << 8) + val.frac_part)) / 256.0f;
    } else {
        retVal = (((uint16_t)val.int_part << 8) + val.frac_part) / 256.0f;
    }
    return retVal;
}

/**
  * converts float to f88_t
  * @param val input value
  * @return f88_t value
  */
f88_t FloatTof88(float val) {

    f88_t   retVal;
    // saturation
    val = (val > 127.996094f) ? 127.996094f : val;
    val = (val < -128.000000f) ? -128.000000f : val;
    int32_t a = (int32_t)(val * 256);
    retVal.int_part = (uint8_t)(a >> 8u);
    retVal.frac_part = (uint8_t)(a);
    return retVal;
}

/**
  * checks is val parity even
  * @param val input value
  * @return 0 even, -1 otherwize
  */
int checkParity32(uint32_t val) {
    uint8_t     onesCount = 0;

    for (size_t j = 0; j < 32U; j++) {
        if ((val & 1u) == 1u) {
            onesCount++;
        }
        val = (val >> 1);
    }
    if ((onesCount & 1u) == 1u) {
        return  -1;
    }
    return 0;
}


/**
  * returns pointer to the requsested MessageTbl entry
  * @param msgId requested Id
  * @return NULL or pointer to the entry
  */

const opentThermMsg_t * GetMessageTblEntry(const uint8_t msgId) {
    size_t i;
    for (i = 0u; i < MsgTblLength; i++) {
        if (msgId == messagesTbl[i].msgId) {
            return &(messagesTbl[i]);
        }
    }
    return NULL;
}
