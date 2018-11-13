/**
  ******************************************************************************
  * @file    opentherm_json.c
  * @author  turchenkov@gmail.com
  * @version V0.0.1
  * @date    07-Nov-2018
  * @brief   This file contains opentherm to json converter functions
  ******************************************************************************
  */

#include <stdint.h>
#include <stddef.h>

#include "opentherm_json.h"
#include "opentherm_data_def.h"
#include "opentherm_daq_def.h"
#include "hex_gen.h"
#include "ftoa.h"

static const uint8_t topic_json0[] = { "{\"Id\":\"" };
#define SIZ0 sizeof(topic_json0)

static const uint8_t topic_json1[] = { "\", \"Val\":\"" };
#define SIZ2 sizeof(topic_json1)

static const uint8_t topic_json2[] = { "\", \"Qual\":\"" };
#define SIZ4 sizeof(topic_json2)

static const uint8_t topic_json3[] = { "\", \"Ts\":\"" };
#define SIZ6 sizeof(topic_json3)

static const uint8_t qgood[] = { "good" };
static const uint8_t qngood[] = { " bad" };
#define SIZ5 sizeof(qngood)

static uint8_t ldids[] = { "$$$" };
#define SIZ1 sizeof(ldids)

static uint8_t szival[] = { "-9876543210" };
#define SIZ3 sizeof(szival)

static uint8_t timest[] = { "xxxxxxxxxx." };
#define SIZ7 sizeof(timest)

static uint8_t timestms[] = { "xxx\"}" };
#define SIZ8 sizeof(timestms)

static uint8_t
	outstring[SIZ0 + SIZ1 + SIZ2 + SIZ3 + SIZ4 + SIZ5 + SIZ6 + SIZ7 + SIZ8];





/**
 * @brief ConvertMVToJSON
 * @param pMV pointer to the MV to be JSON'ized
 * @param outstr pointer to the resulting JSON string
 */
uint8_t * ConvertMVToJSON(tMV *pMV)
{
	if (pMV == NULL) {
		outstring[0] = 0x00;
		goto fExit;
	}
	uint8_t *pdst;
	const uint8_t *psrc;

	if ((pMV->ReportType != GI) && (pMV->ReportType != SP_GI)) {
		outstring[0] = 0x00;
		goto fExit;
	}

	/* generate topic */
	pdst = outstring; /* move pointer to start */
	psrc = topic_json0;
	while (*psrc != 0x00U) {
		*pdst = *psrc;
		pdst++;
		psrc++;
	}

	uint8_to_asciiz(pMV->LD_ID, (char *)ldids);
	psrc = ldids;
	while (*psrc != 0x00U) {
		*pdst = *psrc;
		pdst++;
		psrc++;
	}

	psrc = topic_json1; /* value */
	while (*psrc != 0x00U) {
		*pdst = *psrc;
		pdst++;
		psrc++;
	}

	switch (pMV->MV_type) {
	case intMV:
		int32_to_asciiz(pMV->Val.iVal, (char *)szival);
		break;
	case floatMV:
		ftoa((double)pMV->Val.fVal, (char *)szival, 4);
		break;
	default:
		break;
	}
	psrc = (uint8_t *)szival;
	while (*psrc != 0x00U) {
		*pdst = *psrc;
		pdst++;
		psrc++;
	} /* ival/fval */

	psrc = topic_json2; /* quality */
	while (*psrc != 0x00U) {
		*pdst = *psrc;
		pdst++;
		psrc++;
	}

	if (pMV->Quality == QUALITY_VALIDITY_GOOD) {
		psrc = qgood;
	} else {
		psrc = qngood;
	}
	while (*psrc != 0x00U) {
		*pdst = *psrc;
		pdst++;
		psrc++;
	}		    /* good/bad */
	psrc = topic_json3; /* timestamp */
	while (*psrc != 0x00U) {
		*pdst = *psrc;
		pdst++;
		psrc++;
	}

	uint32_to_asciiz(pMV->TimeStamp.Seconds, (char *)timest);
	psrc = (uint8_t *)timest; /* timestamp */
	while (*psrc != 0x00U) {
		*pdst = *psrc;
		pdst++;
		psrc++;
	}

	uint16_to_asciiz(pMV->TimeStamp.mSeconds,
			 (char *)timestms); /* milliseconds */
	psrc = (uint8_t *)timestms;
	while (*psrc != 0x00U) {
		*pdst = *psrc;
		pdst++;
		psrc++;
	}
	*(pdst - 5U) = *(pdst - 3U);
	*(pdst - 4U) = *(pdst - 2U);
	*(pdst - 3U) = *(pdst - 1U);
	*(pdst - 2U) = (uint8_t)'\"';
	*(pdst - 1U) = (uint8_t)'}';
	*pdst = 0x00U;

fExit:
	return outstring;
}
