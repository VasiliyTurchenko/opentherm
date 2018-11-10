/**
 ******************************************************************************
 * @file    test.c
 * @author  turchenkov@gmail.com
 * @version V0.0.1
 * @date    07-Nov-2018
 * @brief   This file contains test routines for opentherm.c
 ******************************************************************************
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"

#include <unistd.h>

#include "assert.h"
#include "opentherm_data_def.h"
#include "opentherm.h"
#include "message_table.h"
#include "opentherm_daq_def.h"
#include "opentherm_test.h"

static void f88ToFloat_test(void);
static void getTime_test(void);
static void OPENTHERM_PutDataToMsg_test(void);

static const char *ErrMsgs[] = { "resOK\0",	"resBadMsgId\0",
				 "resNoAnswer\0",  "resBadArg\0",
				 "resParityErr\0", "resBadMsgType\0",
				 "resNoMVorSPS\0", "???\0" };

void run_opentherm_test(void)
{
	printf("%s\n", "opentherm_test started...");

	f88ToFloat_test();
	getTime_test();
	OPENTHERM_PutDataToMsg_test();

	printf("%s\n", "opentherm_test finished...");
}

/**
 * @brief f88ToFloat_test
 */
static void f88ToFloat_test(void)
{
	for (size_t ipart = 0; ipart < 0x100; ipart++) {
		for (size_t fpart = 0; fpart < 0x100; fpart++) {
			f88_t arg = { ipart, fpart };
			f88_t ans = FloatTof88(F88ToFloat(arg));

			if ((arg.int_part != ans.int_part) ||
			    (arg.frac_part != ans.frac_part)) {
				printf("%s", "Error\n\0");
				printf("given        : %02X : %02X\n",
				       arg.int_part, arg.frac_part);
				printf("converted to: %02X : %02X\n",
				       ans.int_part, ans.frac_part);
			}
		}
	}
}

/**
 * @brief getTime_test
 */
static void getTime_test(void)
{
	tTime t0;
	tTime t1;
	printf("%s\n", "getTime_test started...");

	t0 = getTime();
	printf("t0 = %d.%d\n", t0.Seconds, t0.mSeconds);
	sleep(1u);
	t1 = getTime();
	printf("t1 = %d.%d\n", t1.Seconds, t1.mSeconds);

	printf("%s\n", "getTime_test finished...");
}

/**
 * @brief OPENTHERM_PutDataToMsg_test
 */
static void OPENTHERM_PutDataToMsg_test()
{
	static tMV Master_MV_array[MV_ARRAY_LENGTH];
	static tMV(*const pMaster_MV_array)[MV_ARRAY_LENGTH] = &Master_MV_array;

	static tMV Slave_MV_array[MV_ARRAY_LENGTH];
	static tMV(*const pSlave_MV_array)[MV_ARRAY_LENGTH] = &Slave_MV_array;

	printf("%s\n", ">>>>>>>>> OPENTHERM_PutDataToMsg_test started...");

	if (OPENTHERM_InitMV(pMaster_MV_array, MV_ARRAY_LENGTH) != 0) {
		printf("Test failed at %s, %s!\n", __FILE__, __FILE__);
		return;
	}

	if (OPENTHERM_InitMV(pSlave_MV_array, MV_ARRAY_LENGTH) != 0) {
		printf("Test failed at %s, %s!\n", __FILE__, __FILE__);
		return;
	}

	/* fill master MV table */
	for (size_t i = 0; i < MV_ARRAY_LENGTH; i++) {
		printf("\ttest msgId = %d\n", Master_MV_array[i].LD_ID);

		if (Master_MV_array[i].MV_type == intMV) {
			Master_MV_array[i].Val.iVal = (int32_t)i;
		} else {
			Master_MV_array[i].Val.fVal =
				(float)i + (float)i / 100.0f;
		}

		uint32_t message;
		openThermResult_t res1 =
			OPENTHERM_PutDataToMsg(&(Master_MV_array[i]), &message);
		if (res1 != OPENTHERM_ResOK) {
			printf("Test failed at %s, %s! (i = %d)\n", __FILE__,
			       __FILE__, i);
			return;
		}
		res1 = OPENTHERM_SaveToMV(&(Slave_MV_array[i]), &message);
		if (res1 != OPENTHERM_ResOK) {
			printf("Test failed at %s, %s! (i = %d)\n", __FILE__,
			       __FILE__, i);
			return;
		}

		/*compare two MVs */
		int errflag = 0;
		if (Master_MV_array[i].MV_type == intMV) {
			errflag = (Master_MV_array[i].Val.iVal ==
				   Slave_MV_array[i].Val.iVal) ?
					  0 :
					  -1;
		} else {
			errflag = (fabs(Master_MV_array[i].Val.fVal -
					Slave_MV_array[i].Val.fVal) < 0.01f) ?
					  0 :
					  -1;
		}
		if (errflag != 0) {
			printf("Test failed at %s, %s! (i = %d)\n", __FILE__,
			       __FILE__, i);
			return;
		}
	}
}
