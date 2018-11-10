/**
  ******************************************************************************
  * @file    opentherm_daq_def.h
  * @author  turchenkov@gmail.com
  * @version V0.0.1
  * @date    08-Nov-2018
  * @brief   This file contains data structures definitions for opentherm protocol to json conversion
  ******************************************************************************
  */

#ifndef OPENTHERM_DAQ_DEF_H
#define OPENTHERM_DAQ_DEF_H

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

#define LD_NAME_MAX_LEN (uint8_t)0x07
#undef USE_LD_NAMES_INSTEAD_OF_IDS

// values definitions
#define SPS_VAL_OFF	(uint8_t)0x00
#define SPS_VAL_ON	(uint8_t)0x01

#define DPS_VAL_INTERMEDIATE (uint8_t)0x00
#define DPS_VAL_OFF	(uint8_t)0x01
#define DPS_VAL_ON	(uint8_t)0x02
#define DPS_VAL_BAD	(uint8_t)(DPS_VAL_ON | DPS_VAL_OFF)

/**
 * @defgroup QUALITY Definitions and functions related to data attribute quality
 */
// clang-format off
#define QUALITY_VALIDITY_GOOD		(uint16_t)0x0000
#define QUALITY_VALIDITY_RESERVED	(uint16_t)0x0001
#define QUALITY_VALIDITY_INVALID	(uint16_t)0x0002
#define QUALITY_VALIDITY_QUESTIONABLE                                          \
	(QUALITY_VALIDITY_RESERVED | QUALITY_VALIDITY_INVALID)
#define QUALITY_DETAIL_OVERFLOW		(uint16_t)0x0004
#define QUALITY_DETAIL_OUT_OF_RANGE	(uint16_t)0x0008
#define QUALITY_DETAIL_BAD_REFERENCE	(uint16_t)0x0010
#define QUALITY_DETAIL_OSCILLATORY	(uint16_t)0x0020
#define QUALITY_DETAIL_FAILURE		(uint16_t)0x0040
#define QUALITY_DETAIL_OLD_DATA		(uint16_t)0x0080
#define QUALITY_DETAIL_INCONSISTENT	(uint16_t)0x0100
#define QUALITY_DETAIL_INACCURATE	(uint16_t)0x0200
#define QUALITY_SOURCE_SUBSTITUTED	(uint16_t)0x0400
#define QUALITY_TEST			(uint16_t)0x0800
#define QUALITY_OPERATOR_BLOCKED	(uint16_t)0x1000
// clang-format on
/* time structure */
typedef struct __attribute__((packed)) _tTime { /*!< time holding structure */
	uint32_t Seconds;			/*!< NTP seconds */
	uint16_t mSeconds;			/*!< milliseconds */
} tTime;
typedef tTime *ptr_Time;

/* Structures used in the module ---------------------------------------------*/

enum tChanged /*!< Is LD changed after last Get_LD_xx() */
{ cpad,       /*!< skip zero */
  Yes,
  No };

#define tControllable tChanged /*!< is LD controllable ? */

enum tRegistered /*!< enum for MQTT-SN registration state  */
{ rpad,		 /*!< skip zero */
  Regd,
  NotRegd };

enum tPubSub /*!< topic for publishing or for subscribing */
{ pspad,     /*!< skip zero */
  Pub,
  Sub };

enum tReport /*!< Type of the LD - spontaneous or(and) interrogated */
{ Inact,     /*!< Inactive */
  GI,	/*!< interrogation */
  SP,	/*!< spontaneous */
  SP_GI      /*!< spont. and interr.*/
};

/* Single Point Indication */
typedef struct _tSPS { /* Single point record definition */
	tTime TimeStamp;
	uint16_t TopicId;    /*!< MQTT-SN short topic id if registered */
	uint16_t SubTopicId; /*!< MQTT-SN short topic id if subscribed */
	uint16_t PacketId;   /*!< Packet Id of the last processed packet */
	uint16_t Quality;
	uint8_t LD_ID;
	uint8_t Val;
	enum tChanged ValChanged;
	enum tReport ReportType;
	enum tRegistered MQTTSNRegState; /*!< Registered or non on the gateway*/
	enum tControllable Ctrl;	 /*!< Controlable or not */
} tSPS;

typedef tSPS *ptr_SPS;

#define tSPS_size sizeof(tSPS)

/*  Double Point Indication  */
typedef struct _tDPS {
	tTime TimeStamp;
	uint16_t TopicId;    /*!< MQTT-SN short topic id if registered */
	uint16_t SubTopicId; /*!< MQTT-SN short topic id if subscribed */
	uint16_t PacketId;   /*!< Packet Id of the last processed packet */
	uint16_t Quality;
	uint8_t LD_ID;
	uint8_t Val;
	enum tChanged ValChanged;
	enum tReport ReportType;
	enum tRegistered MQTTSNRegState; /*!< Registered or non on the gateway */
	enum tControllable Ctrl;	 /*!< Controlable or not */
} tDPS;

typedef tDPS *ptr_DPS;

#define tDPS_size sizeof(tDPS)

/* Measured Value Data */
union MV_Val {
	int32_t iVal; // Integer value
	float fVal;   // float value
};

enum tVal // kind of MV_Val
{ intMV,
  floatMV };
enum tMeas // unit of phys.measurement
{ m,
  kg,
  s,
  A,
  Kelv, // kelvins
  Cels, // Celsius
  V,
  W,
  kW,
  VA,
  VAR,
  percent,
  Pa,
  bar,
  lpm, //litres per minute
  Hz,
  Unnamed,
  Hours,
  BitField // bit field
};

typedef struct _tMV {
	tTime		TimeStamp;
	uint16_t	TopicId;    /*!< MQTT-SN short topic id if registered */
	uint16_t	SubTopicId; /*!< MQTT-SN short topic id if subscribed */
	uint16_t	PacketId;   /*!< Packet Id of the last processed packet */
	uint16_t	Quality;
	uint8_t		LD_ID;
	union MV_Val	Val;
	union MV_Val
			Lowest;		// Lowest permissible value (for quality, not for logic)
	union MV_Val
			Highest;	// Highest permissible value (for quality, not for logic)
	union MV_Val	MV_Aperture;	// for sporadic messages
	union MV_Val	MV_Scale;	// conversion factor
	union MV_Val	MV_Shift;	// zero point shift
	enum tVal	MV_type;
	enum tMeas	MV_Unit;
	enum tChanged	ValChanged;
	enum tReport	ReportType;
	enum tRegistered	MQTTSNRegState; /*!< Registered or non on the gateway*/
	enum tControllable	Ctrl;	/*!< Controlable or not */
} tMV;

typedef tMV *ptr_MV;

#define tMV_size sizeof(tMV)

#endif // OPENTHERM_DAQ_DEF_H
