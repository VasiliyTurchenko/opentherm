/**
  ******************************************************************************
  * @file    test.c
  * @author  turchenkov@gmail.com
  * @version V0.0.1
  * @date    07-Nov-2018
  * @brief   This file contains test routines for opentherm.c
  ******************************************************************************
  */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include    "config.h"

#include "opentherm.h"
#include "opentherm_test.h"
#include "opentherm_daq_def.h"
#include "assert.h"

static void f88ToFloat_test(void);
static void OPENTHERM_SerializeMsg_test(void);

static const char *ErrMsgs[] = {
    "resOK\0",
    "resBadMsgId\0",
    "resNoAnswer\0",
    "resBadArg\0",
    "resParityErr\0",
    "resBadMsgType\0",
    "resNoMVorSPS\0",
    "???\0"
};



void run_opentherm_test(void) {
    printf("%s\n", "opentherm_test started...");

    f88ToFloat_test();

    // serialize message
    OPENTHERM_SerializeMsg_test();

    printf("%s\n", "opentherm_test finished...");
}


static void f88ToFloat_test(void) {
//    f88_t   one = {0x15, 0x80};
//    float   oneAnswer = 21.5f;
//    f88_t   two = {0xFA, 0xC0};
//    float  twoAnswer = -5.25f;
//    if (f88ToFloat(one) == oneAnswer) {
//        printf("%f == %f\n", f88ToFloat(one), oneAnswer);
//    }

//    if (f88ToFloat(two) == twoAnswer) {
//        printf("%f == %f\n", f88ToFloat(two), twoAnswer);
//    }

//    f88_t   t = {0x80, 0x00};
//    printf("%f\n", f88ToFloat(t));
//    f88_t t_o = FloatTof88(f88ToFloat(t));
//    printf("given        : %02X : %02X\n", t.int_part, t.frac_part);
//    printf("converted to: %02X : %02X\n", t_o.int_part, t_o.frac_part);



//    f88_t   t1 = {0x7f, 0xff};
//    printf("%f\n", f88ToFloat(t1));

//    f88_t t1_o = FloatTof88(f88ToFloat(t1));
//    printf("given        : %02X : %02X\n", t1.int_part, t1.frac_part);
//    printf("converted to: %02X : %02X\n", t1_o.int_part, t1_o.frac_part);


    for (size_t ipart = 0; ipart < 0x100; ipart++) {
        for (size_t fpart = 0; fpart < 0x100; fpart++) {
            f88_t arg = {ipart, fpart};
            f88_t ans = FloatTof88(f88ToFloat(arg));

            if ( (arg.int_part != ans.int_part) || (arg.frac_part != ans.frac_part) ) {
                printf("%s", "Error\n\0");
                printf("given        : %02X : %02X\n", arg.int_part, arg.frac_part);
                printf("converted to: %02X : %02X\n", ans.int_part, ans.frac_part);
            }
        }
    }


}

static void OPENTHERM_SerializeMsg_test(void) {

    printf("\nOPENTHERM_SerializeMsg_test started..\n");

    uint8_t     msgType = MSG_TYPE_READ_ACK;
    uint32_t    message;

    for (size_t i = 0; i < MsgTblLength; i++) {

        const opentThermMsg_t * pTableEntry = GetMessageTblEntry(MV_array[i].LD_ID);
        if (pTableEntry == NULL) {
            printf("Test failed at Serialization step %d with error: pTableEntry == NULL \n", i);
            continue;
        }
        switch (pTableEntry->msgDataType1) {
            case u8:
            case fl8:
            case u16:
            case none:
                MV_array[i].Val.iVal = (int32_t)i;
                break;
            case s8:
            case s16:
                MV_array[i].Val.iVal = (int32_t)i;
                break;
            case f88:
                MV_array[i].Val.fVal = ((float)i + (float)i / 100.0f);
                break;
        }

        openThermResult_t r = OPENTHERM_SerializeMsg(&message, &(MV_array[i]), msgType);

        if (r != resOK) {
            printf("Test failed at Serialization step %d with error %s\n", i, ErrMsgs[r]);
        }

        int32_t ival = MV_array[i].Val.iVal;
        float   fval = MV_array[i].Val.fVal;

        r = OPENTHERM_DeserializeMsg(message, &MV_array , MsgTblLength);

        if (r != resOK) {
            printf("Test failed at Deserialization step %d with error %s\n", i, ErrMsgs[r]);
        }

        int rr = 0;
        if (MV_array[i].MV_type == intMV) {
            if (MV_array[i].Val.iVal != ival) {
                rr = -1;
            }
        } else {
            if ( fabs(MV_array[i].Val.fVal - fval) > 0.005 ) {
                rr = -1;
            }
        }

        if ( rr == -1 ) {
            printf("Test failed at Deserialization step %d (msgId = %d) with error: wrong MV_array[i].Val\n", i, pTableEntry->msgId);
            printf("\tMV_array[i].Val.fVal = %f , deserialized to  %f \n\n", fval, MV_array[i].Val.fVal);
        }
    }
}
