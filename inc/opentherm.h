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
#include    "opentherm_version.h"
#include    "opentherm_daq_def.h"

#define OPENTHERM_VERSION_MAJOR 0
#define OPENTHERM_VERSION_MINOR 0
#define OPENTHERM_VERSION_PATCH 1

#define OPENTHERM_STANDART_MAJOR 2
#define OPENTHERM_STANDART_MINOR 2

/*
 * @brief   OpenTherm   msg types
 */
#define MSG_TYPE_READ_DATA      (uint8_t)0
#define MSG_TYPE_WRITE_DATA     (uint8_t)1
#define MSG_TYPE_INVALID_DATA   (uint8_t)2
#define MSG_TYPE_RESERVED       (uint8_t)3
#define MSG_TYPE_READ_ACK       (uint8_t)4
#define MSG_TYPE_WRITE_ACK      (uint8_t)5
#define MSG_TYPE_DATA_INVALID   (uint8_t)6
#define MSG_TYPE_UNKN_DATAID    (uint8_t)7

/* first byte is composed of parity, 3 bits of msgType and 4 reserved bits */
typedef struct __attribute__((packed)) firstByte {
        uint8_t    reserved : 4;
        uint8_t    msgType  : 3;
        uint8_t    parity   : 1;
} firstByte_t;

/*
 * @brief   OpenTherm   data types
 */
/* f8.8 signed fixed point value : 1 sign bit, 7 integer bit,
 * 8 fractional bits (two’s compliment ie. the LSB of the 16bit binary number represents 1/256th of a unit).
*/
typedef struct __attribute__((packed)) f88 {
        uint8_t  int_part;
        uint8_t frac_part;
} f88_t;

/* flag8 byte composed of 8 single-bit flags */
typedef struct __attribute__((packed)) flag8 {
        uint8_t    bit0 : 1;
        uint8_t    bit1 : 1;
        uint8_t    bit2 : 1;
        uint8_t    bit3 : 1;
        uint8_t    bit4 : 1;
        uint8_t    bit5 : 1;
        uint8_t    bit6 : 1;
        uint8_t    bit7 : 1;
} flag8_t;

typedef struct __attribute__((packed)) dataValueFlags {
        flag8_t flags1;
        flag8_t flags2;
} dataValueFlags;

/*
  * @brief  OpenTherm frame
  */
typedef struct __attribute__((packed)) dataValueBytesU {
        uint8_t dataByte1;
        uint8_t dataByte2;
} dataValueBytesU;

typedef struct __attribute__((packed)) dataValueBytesS {
        int8_t dataByte1;
        int8_t dataByte2;
} dataValueBytesS;

typedef struct __attribute__((packed)) openThermFrame {
    firstByte_t		byte1;
    uint8_t         dataID;
    union {
        dataValueBytesU      dataValueBU;
        dataValueBytesS      dataValueBS;
        uint16_t            dataValueU16;
        int16_t             dataValueS16;
        f88_t               dataValueF88;
        dataValueFlags      dataValueFl;
    } dataVal;
} openThermFrame_t;

typedef openThermFrame_t * openThermFrame_p;

#define MSG_ID_STATUS           (uint8_t)0      /* R -  Status flag8 / flag8 Master and Slave Status flags. */
#define MSG_ID_TSET             (uint8_t)1      /* - W TSet f8.8 Control setpoint ie CH water temperature setpoint (°C) */
#define MSG_ID_M_CONFIG         (uint8_t)2      /* - W M-Config / M-MemberIDcode flag8 / u8 Master Configuration Flags / Master MemberID Code */
#define MSG_ID_S_CONFIG         (uint8_t)3      /* R - S-Config / S-MemberIDcode flag8 / u8 Slave Configuration Flags / Slave MemberID Code */
#define MSG_ID_COMMAND          (uint8_t)4      /* - W Command u8 / u8 Remote Command */
#define MSG_ID_ASF_FLAGS        (uint8_t)5      /* R - ASF-flags / OEM-fault-code flag8 / u8 Application-specific fault flags and OEM fault code */
#define MSG_ID_RBP_FLAGS        (uint8_t)6      /* R - RBP-flags flag8 / flag8 Remote boiler parameter transfer-enable & read/write flags */
#define MSG_ID_COOLING_CONTROL  (uint8_t)7      /* - W Cooling-control f8.8 Cooling control signal (%) */
#define MSG_ID_TSETCH2          (uint8_t)8      /* - W TsetCH2 f8.8 Control setpoint for 2e CH circuit (°C) */
#define MSG_ID_OVERRIDE         (uint8_t)9      /* R - TrOverride f8.8 Remote override room setpoint */
#define MSG_ID_TSP              (uint8_t)10     /* R - TSP u8 / u8 Number of Transparent-Slave-Parameters supported by slave */
#define MSG_ID_TSP_INDEX        (uint8_t)11     /* R W TSP-index / TSP-value u8 / u8 Index number / Value of referred-to transparent slave parameter */
#define MSG_ID_FHB_SIZE         (uint8_t)12     /* R - FHB-size u8 / u8 Size of Fault-History-Buffer supported by slave */
#define MSG_ID_FHB_INDEX        (uint8_t)13     /* R - FHB-index / FHB-value u8 / u8 Index number / Value of referred-to fault-history buffer entry */
#define MSG_ID_MAX_REL_MOD_LEV_SETTING  (uint8_t)14 /* - W Max-rel-mod-level-setting f8.8 Maximum relative modulation level setting (%) */
#define MSG_ID_MAX_CAPACITY     (uint8_t)15     /* R - Max-Capacity / Min-Mod-Level u8 / u8 Maximum boiler capacity (kW) / Minimum boiler modulation level(%) */

#define MSG_ID_TRSET            (uint8_t)16     /* - W TrSet f8.8 Room Setpoint (°C) */
#define MSG_ID_REL_MOD_LEVEL    (uint8_t)17     /* R - Rel.-mod-level f8.8 Relative Modulation Level (%) */
#define MSG_ID_CH_PRESSURE      (uint8_t)18     /* R - CH-pressure f8.8 Water pressure in CH circuit (bar) */
#define MSG_ID_DHW_FLOW_RATE    (uint8_t)19     /* R - DHW-flow-rate f8.8 Water flow rate in DHW circuit. (litres/minute) */
#define MSG_ID_DAY_TIME         (uint8_t)20     /* R W Day-Time special / u8 Day of Week and Time of Day */
#define MSG_ID_DATE             (uint8_t)21     /* R W Date u8 / u8 Calendar date */
#define MSG_ID_YEAR             (uint8_t)22     /* R W Year u16 Calendar year */
#define MSG_ID_TRSETCH2         (uint8_t)23     /* - W TrSetCH2 f8.8 Room Setpoint for 2nd CH circuit (°C) */
#define MSG_ID_TR               (uint8_t)24     /* - W Tr f8.8 Room temperature (°C) */
#define MSG_ID_TBOILER          (uint8_t)25     /* R - Tboiler f8.8 Boiler flow water temperature (°C) */
#define MSG_ID_TDHW             (uint8_t)26     /* R - Tdhw f8.8 DHW temperature (°C) */
#define MSG_ID_TOUTSIDE         (uint8_t)27     /* R - Toutside f8.8 Outside temperature (°C) */
#define MSG_ID_TRET             (uint8_t)28     /* R - Tret f8.8 Return water temperature (°C) */
#define MSG_ID_TSTORAGE         (uint8_t)29     /* R - Tstorage f8.8 Solar storage temperature (°C) */
#define MSG_ID_TCOLLECTOR       (uint8_t)30     /* R - Tcollector f8.8 Solar collector temperature (°C) */
#define MSG_ID_TFLOWCH2         (uint8_t)31     /* R - TflowCH2 f8.8 Flow water temperature CH2 circuit (°C) */

#define MSG_ID_TDHW2            (uint8_t)32     /* R - Tdhw2 f8.8 Domestic hot water temperature 2 (°C) */
#define MSG_ID_TEXHAUST         (uint8_t)33     /* R - Texhaust s16 Boiler exhaust temperature (°C) */
#define MSG_ID_TDHWSET_UB_LB    (uint8_t)48     /* R - TdhwSet-UB / TdhwSet-LB s8 / s8 DHW setpoint upper & lower bounds for adjustment (°C) */
#define MSG_ID_MAXTSET_UB_LB    (uint8_t)49     /* R - MaxTSet-UB / MaxTSet-LB s8 / s8 Max CH water setpoint upper & lower bounds for adjustment (°C) */
#define MSG_ID_HCRATIO_UB_LB    (uint8_t)50     /* R - Hcratio-UB / Hcratio-LB s8 / s8 OTC heat curve ratio upper & lower bounds for adjustment */
#define MSG_ID_TDHWSET          (uint8_t)56     /* R W TdhwSet f8.8 DHW setpoint (°C) (Remote parameter 1)*/
#define MSG_ID_MAXTSET          (uint8_t)57     /* R W MaxTSet f8.8 Max CH water setpoint (°C) (Remote parameters 2) */
#define MSG_ID_HCRATIO          (uint8_t)58     /* R W Hcratio f8.8 OTC heat curve ratio (°C) (Remote parameter 3)*/

#define MSG_ID_REMOTE_OVERRIDE      (uint8_t)100    /* R - Remote override function flag8 / - Function of manual and program changes in master and remote room setpoint */
#define MSG_ID_OEM_DIAG_CODE        (uint8_t)115    /* R - OEM diagnostic code u16 OEM-specific diagnostic/service code */
#define MSG_ID_BURNER_STARTS        (uint8_t)116    /* R W Burner starts u16 Number of starts burner */
#define MSG_ID_CH_PUMP_STARTS       (uint8_t)117    /* R W CH pump starts u16 Number of starts CH pump */
#define MSG_ID_DHW_PUMP_STARTS      (uint8_t)118    /* R W DHW pump/valve starts u16 Number of starts DHW pump/valve */
#define MSG_ID_DHW_BURNER_STARTS    (uint8_t)119    /* R W DHW burner starts u16 Number of starts burner during DHW mode */

#define MSG_ID_BURNER_OP_HOURS      (uint8_t)120    /* R W Burner operation hours u16 Number of hours that burner is in operation (i.e. flame on) */
#define MSG_ID_CH_PUMP_OP_HOURS     (uint8_t)121    /* R W CH pump operation hours u16 Number of hours that CH pump has been running */
#define MSG_ID_DHW_PUMP_OP_HOURS    (uint8_t)122    /* R W DHW pump/valve operation hours u16 Number of hours that DHW pump has been running or DHW valve has been opened */
#define MSG_ID_DHW_BURNER_OP_HOURS  (uint8_t)123    /* R W DHW burner operation hours u16 Number of hours that burner is in operation during DHW mode */
#define MSG_ID_OPENTHERM_VER_MASTER (uint8_t)124    /* - W OpenTherm version Master f8.8 The implemented version of the OpenTherm Protocol Specification in the master. */
#define MSG_ID_OPENTHERM_VER_SLAVE  (uint8_t)125    /* R - OpenTherm version Slave f8.8 The implemented version of the OpenTherm Protocol Specification in the slave. */
#define MSG_ID_MASTER_VER           (uint8_t)126    /* - W Master-version u8 / u8 Master product version number and type */
#define MSG_ID_SLAVE_VER            (uint8_t)127    /* R - Slave-version u8 / u8 Slave product version number and type */


/* Messages template */
typedef enum {
    dummy0,
    rd,         /* read only message */
    wr,         /* write only message */
    rw        /* read and write message */
} msgMode_t;

typedef enum {
    none,
    fl8,        /* bit field */
    u8,         /* uint8_t */
    s8,         /* int8_t */
    f88,        /* fixed point decimal */
    u16,        /* uint16_t */
    s16         /* int16_t */
} msgData_t;

typedef struct __attribute__((packed)) opentThermMsg {
    uint8_t     msgId;
    msgMode_t   msgMode;
    msgData_t   msgDataType1;   /* type of data for the first byte */
    msgData_t   msgDataType2;   /* type of data for the 2nd byte */
    uint8_t    msgClass;

    /* for internal data encoding */
    enum        tMeas       MV_Unit1;       /* unit of measurements for 1st val */
    enum        tMeas       MV_Unit2;       /* unit of measurements for 2nd val */
    union       MV_Val      Lowest;         /* Lowest permissible value (for quality, not for logic) */
    union       MV_Val      Highest;        /* Highest permissible value (for quality, not for logic) */
} opentThermMsg_t;


/* result codes */
typedef enum {
    resOK,
    resBadMsgId,          /* bad msg ID */
    resNoAnswer,        /* no answer from slave */
    resBadArg,          /* bad arguments */
    resParityErr,       /* parity error */
    resBadMsgType,      /* bad message type */
    resNoMVorSPS,       /* no MV or SPS configured for the msgId*/
} openThermResult_t;


/*
 * Functions prototypes
*/
int32_t OPENTHERM_InitMV(tMV (* const pMVArr)[], size_t MVArrLen);

openThermResult_t OPENTHERM_DeserializeMsg(uint32_t msg, tMV (* const pMVArr)[], const size_t MVArrLen);

openThermResult_t OPENTHERM_SerializeMsg(uint32_t * const pmsg, tMV * const pMV, uint8_t msgType);

const opentThermMsg_t * GetMessageTblEntry(const uint8_t msgId);

float f88ToFloat(f88_t val);
f88_t FloatTof88(float val);
/*
 * Exported variables
 */

extern const    uint8_t             MsgTblLength;
extern          tMV	MV_array[];

#endif // OPENTHERM_H
