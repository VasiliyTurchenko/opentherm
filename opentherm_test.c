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

#include "opentherm_master.h"
#include "opentherm_slave.h"

/**
  * @brief states for test communication emulation
  */
typedef enum {
	commStateIdle,
	commStateMasterWaitsForReply,
	commStateSlaveSentReply,
	commStateRelpyRcvd,
} commState;

static void f88ToFloat_test(void);
static void getTime_test(void);
static void OPENTHERM_PutDataToMsg_test(void);
static void OPENTHERM_PutDataToMsg_Errors_test(void);
static void OPENTHERM_OPENTHERM_SaveToMV_Errors_test(void);
static void checkMVs(void);

static void init_MVArrays(void);

/* full circuit test */
static void OPENTHERM_WriteSlave_ReadSlave_Test(void);
static uint32_t Master_CommFun(uint32_t message);
static tMV *GetSlaveMV(uint8_t LdId);
static void fullCircuitTest(uint8_t msgId, union MV_Val val, union MV_Val val2);

static void OPENTHERM_ReadOnlyTest(void);
static void OPENTHERM_WriteOnlyTest(void);

/* auxiliary functions */
void PrintMVInfo(tMV *const pMV);

/* static data */
static tMV Master_MV_array[MV_ARRAY_LENGTH];
static tMV (*const pMaster_MV_array)[MV_ARRAY_LENGTH] = &Master_MV_array;

static tMV Slave_MV_array[MV_ARRAY_LENGTH];
static tMV (*const pSlave_MV_array)[MV_ARRAY_LENGTH] = &Slave_MV_array;

/************** TESTS *******************************/

/**
 * @brief run_opentherm_test
 */
void run_opentherm_test(void)
{
	printf("%s\n", "opentherm_test started...");

	init_MVArrays();

	checkMVs();

	f88ToFloat_test();
	getTime_test();
	OPENTHERM_PutDataToMsg_Errors_test();
	OPENTHERM_OPENTHERM_SaveToMV_Errors_test();
	OPENTHERM_PutDataToMsg_test();

	OPENTHERM_WriteSlave_ReadSlave_Test();
	OPENTHERM_ReadOnlyTest();
	OPENTHERM_WriteOnlyTest();

	printf("%s\n", "opentherm_test finished...");
}

/**
 * @brief f88ToFloat_test
 */
static void f88ToFloat_test(void)
{
	for (size_t ipart = 0; ipart < 0x100; ipart++) {
		for (size_t fpart = 0; fpart < 0x100; fpart++) {
			f88_t arg = { (uint8_t)ipart, (uint8_t)fpart };
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
 * @brief OPENTHERM_PutDataToMsg_Errors_test
 */
static void OPENTHERM_PutDataToMsg_Errors_test()
{
	printf("%s\n",
	       ">>>>>>>>> OPENTHERM_PutDataToMsg_Errors_test started...");

	tMV Master_MV_array[MV_ARRAY_LENGTH];
	tMV(*const pMaster_MV_array)[MV_ARRAY_LENGTH] = &Master_MV_array;
	if (OPENTHERM_InitMV(pMaster_MV_array, MV_ARRAY_LENGTH) != 0) {
		printf("Test failed at %s: %d!\n", __FILE__, __LINE__);
		return;
	}

	tMV badMV;
	badMV.MV_type = intMV;
	uint32_t message;

	if (OPENTHERM_PutDataToMsg(&badMV, NULL) != OPENTHERM_ResNullPtr) {
		printf("Test failed at %s: %d! \n", __FILE__, __LINE__);
		return;
	}
	if (OPENTHERM_PutDataToMsg(NULL, &message) != OPENTHERM_ResNullPtr) {
		printf("Test failed at %s: %d! \n", __FILE__, __LINE__);
		return;
	}

	badMV.LD_ID = Master_MV_array[MV_ARRAY_LENGTH - 1].LD_ID + 1u;
	if (OPENTHERM_PutDataToMsg(&badMV, &message) != OPENTHERM_ResBadMsgId) {
		printf("Test failed at %s: %d! \n", __FILE__, __LINE__);
		return;
	}
}

/**
 * @brief OPENTHERM_OPENTHERM_SaveToMV_Errors_test
 */
static void OPENTHERM_OPENTHERM_SaveToMV_Errors_test()
{
	printf("%s\n",
	       ">>>>>>>>> OPENTHERM_OPENTHERM_SaveToMV_Errors_test started...");

	tMV Master_MV_array[MV_ARRAY_LENGTH];
	tMV(*const pMaster_MV_array)[MV_ARRAY_LENGTH] = &Master_MV_array;
	if (OPENTHERM_InitMV(pMaster_MV_array, MV_ARRAY_LENGTH) != 0) {
		printf("Test failed at %s: %d!\n", __FILE__, __LINE__);
		return;
	}

	tMV badMV;
	badMV.MV_type = intMV;
	uint32_t message;

	if (OPENTHERM_SaveToMV(&badMV, NULL) != OPENTHERM_ResNullPtr) {
		printf("Test failed at %s: %d! \n", __FILE__, __LINE__);
		return;
	}
	if (OPENTHERM_SaveToMV(NULL, &message) != OPENTHERM_ResNullPtr) {
		printf("Test failed at %s: %d! \n", __FILE__, __LINE__);
		return;
	}

	badMV.LD_ID = Master_MV_array[MV_ARRAY_LENGTH - 1].LD_ID + 1u;
	if (OPENTHERM_SaveToMV(&badMV, &message) != OPENTHERM_ResNoMVorSPS) {
		printf("Test failed at %s: %d! \n", __FILE__, __LINE__);
		return;
	}
}

/**
 * @brief OPENTHERM_PutDataToMsg_test
 */

static void OPENTHERM_PutDataToMsg_test()
{
	//	init_MVArrays();

	//	/* 0 - Master_MV_array[i].Val < Lowest */
	//	/* 1 - Master_MV_array[i].Val in the mid of range */
	//	/* 2 - Master_MV_array[i].Val > Highest */

	//	for (uint32_t testCase = 0; testCase < 3; testCase++) {
	//		printf("OPENTHERM_PutDataToMsg_test startet with testCase %d\n",
	//		       testCase);
	//		for (size_t i = 0; i < MV_ARRAY_LENGTH; i++) {
	//			printf("\ttest msgId = %d\n", Master_MV_array[i].LD_ID);

	//			//		PrintMVInfo(&(Master_MV_array[i]));

	//			const opentThermMsg_t *ptable =
	//				GetMessageTblEntry(Master_MV_array[i].LD_ID);

	//			switch (testCase) {
	//			case 0:
	//				if (Master_MV_array[i].MV_type == intMV) {
	//					int32_t iVal =
	//						(ptable->Lowest.iVal ==
	//						 INT16_MIN) ?
	//							INT16_MIN :
	//							ptable->Lowest.iVal - 1;
	//					Master_MV_array[i].Val.iVal = iVal;
	//				} else {
	//					float fVal =
	//						ptable->Lowest.fVal - 0.004f;
	//					Master_MV_array[i].Val.fVal = fVal;
	//				}
	//				break;
	//			case 1:
	//				if (Master_MV_array[i].MV_type == intMV) {
	//					int32_t iVal = (ptable->Highest.iVal -
	//							ptable->Lowest.iVal) /
	//						       2;
	//					Master_MV_array[i].Val.iVal = iVal;
	//				} else {
	//					float fVal = (ptable->Highest.fVal -
	//						      ptable->Lowest.fVal) /
	//						     2;
	//					Master_MV_array[i].Val.fVal = fVal;
	//				}

	//				break;
	//			case 2:
	//				if (Master_MV_array[i].MV_type == intMV) {
	//					int32_t iVal =
	//						(ptable->Highest.iVal ==
	//						 INT16_MAX) ?
	//							INT16_MAX :
	//							ptable->Highest.iVal +
	//								1;
	//					Master_MV_array[i].Val.iVal = iVal;
	//				} else {
	//					float fVal =
	//						ptable->Highest.fVal + 0.004f;
	//					Master_MV_array[i].Val.fVal = fVal;
	//				}
	//				break;
	//			}

	//			uint32_t message;
	//			openThermResult_t res1 = OPENTHERM_PutDataToMsg(
	//				&(Master_MV_array[i]), &message);
	//			if (res1 != OPENTHERM_ResOK) {
	//				printf("Test failed at %s: %d! (i = %d)\n",
	//				       __FILE__, __LINE__, i);
	//				return;
	//			}
	//			res1 = OPENTHERM_SaveToMV(&(Slave_MV_array[i]), &message);

	//			if ( ((testCase == 0) || (testCase == 2)) && (Slave_MV_array[i].Highest.iVal != INT16_MAX) ) {

	//				if (res1 != OPENTHERM_ResDataInvalid) {
	//					printf("Test failed at %s: %d! (i = %d)\n",
	//					       __FILE__, __LINE__, i);
	//					return;
	//				}

	//			} else {
	//				if (res1 != OPENTHERM_ResOK) {
	//					printf("Test failed at %s: %d! (i = %d)\n",
	//					       __FILE__, __LINE__, i);
	//					return;
	//				}
	//			}

	//			/*compare two MVs */
	//			int errflag = 0;
	//			if (Master_MV_array[i].MV_type == intMV) {
	//				errflag = (Master_MV_array[i].Val.iVal ==
	//					   Slave_MV_array[i].Val.iVal) ?
	//						  0 :
	//						  -1;
	//			} else {
	//				errflag = (fabs(Master_MV_array[i].Val.fVal -
	//						Slave_MV_array[i].Val.fVal) <
	//					   0.01f) ?
	//						  0 :
	//						  -1;
	//			}
	//			if (errflag != 0) {
	//				printf("Test failed at %s: %d! (i = %d)\n",
	//				       __FILE__, __LINE__, i);
	//				return;
	//			}
	//		}
	//	}
}

/**
 * @brief OPENTHERM_WriteSlave_Test
 */
static void OPENTHERM_WriteSlave_ReadSlave_Test(void)
{
	printf("%s\n",
	       ">>>>>>>>> OPENTHERM_WriteSlave_ReadSlave_Test started...");

	init_MVArrays();

	/* test with {.msgId = MSG_ID_YEAR, rw, u16, u16,   4u, Unnamed,  Unnamed,  .Lowest.iVal = 1900,.Highest.iVal = 2100}	*/
	union MV_Val testVal;
	union MV_Val testVal2;

	/* test all the msgIds which can be written and read */
	for (uint32_t i = 0; i < MSG_TBL_LENGTH; i++) {
		if (messagesTbl[i].msgMode != rw) {
			continue;
		}
		uint32_t use2MVs =
			(messagesTbl[i].msgDataType2 != none) ? 2u : 1u;

		printf("testing msgId %d with testVal", messagesTbl[i].msgId);
		if (messagesTbl[i].msgDataType1 == f88) {
			testVal.fVal = messagesTbl[i].Lowest_MV1.fVal +
				       (messagesTbl[i].Highest_MV1.fVal -
					messagesTbl[i].Lowest_MV1.fVal) /
					       2.0f;
			printf(".fVal = %f\n", testVal.fVal);
		} else {
			testVal.iVal = messagesTbl[i].Lowest_MV1.iVal +
				       (messagesTbl[i].Highest_MV1.iVal -
					messagesTbl[i].Lowest_MV1.iVal) /
					       2;
			printf(".iVal = %d\n", testVal.iVal);
		}
		if (use2MVs == 2u) {
			if (messagesTbl[i].msgDataType2 == f88) {
				printf("Test failed at %s: %d!\n", __FILE__,
				       __LINE__);
			} else {
				testVal2.iVal =
					messagesTbl[i].Lowest_MV2.iVal +
					(messagesTbl[i].Highest_MV2.iVal -
					 messagesTbl[i].Lowest_MV2.iVal) /
						2;
				printf("testing msgId %d with testVal2",
				       messagesTbl[i].msgId);
				printf(".iVal = %d\n", testVal2.iVal);
			}
		} else {
			testVal2.iVal = 0;
		}

		fullCircuitTest(messagesTbl[i].msgId, testVal, testVal2);
	}

} /* end of OPENTHERM_WriteSlave_ReadSlave_Test() */

/**
 * @brief readOnlyTest tests reading of the slave RO msgIds
 */
static void OPENTHERM_ReadOnlyTest(void)
{
	printf("%s\n",
	       ">>>>>>>>> OPENTHERM_ReadOnlyTest started...");

	init_MVArrays();
	/* test all the msgIds which are read-only */
	/* prepare slave MV array */
	for (uint32_t i = 0; i < MSG_TBL_LENGTH; i++) {
		if (messagesTbl[i].msgMode != rd) {
			continue;
		}
		uint32_t use2MVs =
			(messagesTbl[i].msgDataType2 != none) ? 2u : 1u;

		tMV *pMV_slave = NULL;
		tMV *pMV2_slave = NULL;

		/* assume that Slave_MV_array and Master_MV_Array have equal length and MVs */
		tMV *pMV_master = NULL;
		tMV *pMV2_master = NULL;

		for (size_t j = 0; j < MV_ARRAY_LENGTH; j++) {
			if (Slave_MV_array[j].LD_ID == messagesTbl[i].msgId) {
				pMV_slave = &Slave_MV_array[j];
				pMV_master = &Master_MV_array[j];
				break;
			}
		}

		if (use2MVs == 2) {
			pMV2_slave = pMV_slave + 1;
			pMV2_master = pMV_master + 1;
		}

		printf("testing read-only msgId %d with testVal",
		       messagesTbl[i].msgId);
		if (messagesTbl[i].msgDataType1 == f88) {
			pMV_slave->Val.fVal = messagesTbl[i].Lowest_MV1.fVal +
					(messagesTbl[i].Highest_MV1.fVal -
					 messagesTbl[i].Lowest_MV1.fVal) /
						2.0f;
			printf(".fVal = %f\n", pMV_slave->Val.fVal);
		} else {
			pMV_slave->Val.iVal = messagesTbl[i].Lowest_MV1.iVal +
					(messagesTbl[i].Highest_MV1.iVal -
					 messagesTbl[i].Lowest_MV1.iVal) /
						2;
			printf(".iVal = %d\n", pMV_slave->Val.iVal);
		}
		if (use2MVs == 2u) {
			if (messagesTbl[i].msgDataType2 == f88) {
				printf("Test failed at %s: %d!\n", __FILE__,
				       __LINE__);
			} else {
				pMV2_slave->Val.iVal =
					messagesTbl[i].Lowest_MV2.iVal +
					(messagesTbl[i].Highest_MV2.iVal -
					 messagesTbl[i].Lowest_MV2.iVal) /
						2;
				printf("testing read-only msgId %d with testVal2",
				       messagesTbl[i].msgId);
				printf(".iVal = %d\n", pMV2_slave->Val.iVal);
			}
		}

		openThermResult_t readSlaveRes;
		readSlaveRes = OPENTHERM_ReadSlave(pMV_master, Master_CommFun);
		if (readSlaveRes != OPENTHERM_ResOK) {
			printf("Test failed at %s: %d!\n", __FILE__, __LINE__);
			printf("Error code %d\n", readSlaveRes);
			exit(-1);
		}
		/* check results */
		int diff1 = 0;

		if (pMV_slave->MV_type == intMV) {
			diff1 = (pMV_slave->Val.iVal != pMV_master->Val.iVal) ? 1 : 0;
		} else {
			diff1 = (fabs((double)pMV_slave->Val.fVal -
				      (double)pMV_master->Val.fVal) > 0.05) ?
					1 :
					0;
		}
		if (diff1 != 0) {
			printf("Read-only test with msgId = %d failed at %s: %d!\n",
			       messagesTbl[i].msgId, __FILE__, __LINE__);
			if (pMV_slave->MV_type == intMV) {
				printf("diff.iVal = %d\n",
				       (pMV_slave->Val.iVal - pMV_master->Val.iVal));
			} else {
				printf("diff.fVal = %f\n",
				       (pMV_slave->Val.fVal - pMV_master->Val.fVal));
			}

			exit(-1);
		} else {
			printf("Read-only test with msgId = %d passed!\n\n",
			       messagesTbl[i].msgId);
		}

		if (use2MVs == 2) {
			if (pMV2_slave->MV_type == intMV) {
				diff1 = (pMV2_slave->Val.iVal !=
					 pMV2_master->Val.iVal) ?
						1 :
						0;
			} else {
				diff1 = (fabs((double)pMV2_slave->Val.fVal -
					      (double)pMV2_master->Val.fVal) >
					 0.05) ?
						1 :
						0;
			}
			if (diff1 != 0) {
				printf("Read-only test with msgId = %d failed at %s: %d!\n",
				       messagesTbl[i].msgId, __FILE__,
				       __LINE__);
				if (pMV2_slave->MV_type == intMV) {
					printf("diff.iVal = %d\n",
					       (pMV2_slave->Val.iVal -
						pMV2_master->Val.iVal));
				} else {
					printf("diff.fVal = %f\n",
					       (pMV2_slave->Val.fVal -
						pMV2_master->Val.fVal));
				}

				exit(-1);
			} else {
				printf("Read-only test with msgId = %d passed!\n\n",
				       messagesTbl[i].msgId);
			}
		}
	}
}

/**
 * @brief OPENTHERM_WriteOnlyTest
 */
static void OPENTHERM_WriteOnlyTest(void) {
	printf("%s\n",
	       ">>>>>>>>> OPENTHERM_WriteOnlyTest started...");

	init_MVArrays();
	/* test all the msgIds which are write-only */
	/* prepare slave MV array */
	for (uint32_t i = 0; i < MSG_TBL_LENGTH; i++) {
		if (messagesTbl[i].msgMode != wr) {
			continue;
		}
		uint32_t use2MVs =
			(messagesTbl[i].msgDataType2 != none) ? 2u : 1u;

		tMV *pMV_master = NULL;
		tMV *pMV2_master = NULL;

		/* assume that Slave_MV_array and Master_MV_Array have equal length and MVs */
		tMV *pMV_slave = NULL;
		tMV *pMV2_slave = NULL;

		for (size_t j = 0; j < MV_ARRAY_LENGTH; j++) {
			if (Slave_MV_array[j].LD_ID == messagesTbl[i].msgId) {
				pMV_master = &Slave_MV_array[j];
				pMV_slave = &Master_MV_array[j];
				break;
			}
		}

		if (use2MVs == 2) {
			pMV2_master = pMV_master + 1;
			pMV2_slave = pMV_slave + 1;
		}

		printf("testing write-only msgId %d with testVal",
		       messagesTbl[i].msgId);
		if (messagesTbl[i].msgDataType1 == f88) {
			pMV_master->Val.fVal = messagesTbl[i].Lowest_MV1.fVal +
					(messagesTbl[i].Highest_MV1.fVal -
					 messagesTbl[i].Lowest_MV1.fVal) /
						2.0f;
			printf(".fVal = %f\n", pMV_master->Val.fVal);
		} else {
			pMV_master->Val.iVal = messagesTbl[i].Lowest_MV1.iVal +
					(messagesTbl[i].Highest_MV1.iVal -
					 messagesTbl[i].Lowest_MV1.iVal) /
						2;
			printf(".iVal = %d\n", pMV_master->Val.iVal);
		}
		if (use2MVs == 2u) {
			if (messagesTbl[i].msgDataType2 == f88) {
				printf("Test failed at %s: %d!\n", __FILE__,
				       __LINE__);
			} else {
				pMV2_master->Val.iVal =
					messagesTbl[i].Lowest_MV2.iVal +
					(messagesTbl[i].Highest_MV2.iVal -
					 messagesTbl[i].Lowest_MV2.iVal) /
						2;
				printf("testing write-only msgId %d with testVal2",
				       messagesTbl[i].msgId);
				printf(".iVal = %d\n", pMV2_master->Val.iVal);
			}
		}

		openThermResult_t writeSlaveRes;
		writeSlaveRes = OPENTHERM_WriteSlave(pMV_slave, Master_CommFun);
		if (writeSlaveRes != OPENTHERM_ResOK) {
			printf("Test failed at %s: %d!\n", __FILE__, __LINE__);
			printf("Error code %d\n", writeSlaveRes);
			exit(-1);
		}
		/* check results */
		int diff1 = 0;

		if (pMV_master->MV_type == intMV) {
			diff1 = (pMV_master->Val.iVal != pMV_slave->Val.iVal) ? 1 : 0;
		} else {
			diff1 = (fabs((double)pMV_master->Val.fVal -
				      (double)pMV_slave->Val.fVal) > 0.05) ?
					1 :
					0;
		}
		if (diff1 != 0) {
			printf("Write-only test with msgId = %d failed at %s: %d!\n",
			       messagesTbl[i].msgId, __FILE__, __LINE__);
			if (pMV_master->MV_type == intMV) {
				printf("diff.iVal = %d\n",
				       (pMV_master->Val.iVal - pMV_slave->Val.iVal));
			} else {
				printf("diff.fVal = %f\n",
				       (pMV_master->Val.fVal - pMV_slave->Val.fVal));
			}

			exit(-1);
		} else {
			printf("Write-only test with msgId = %d passed!\n\n",
			       messagesTbl[i].msgId);
		}

		if (use2MVs == 2) {
			if (pMV2_master->MV_type == intMV) {
				diff1 = (pMV2_master->Val.iVal !=
					 pMV2_slave->Val.iVal) ?
						1 :
						0;
			} else {
				diff1 = (fabs((double)pMV2_master->Val.fVal -
					      (double)pMV2_slave->Val.fVal) >
					 0.05) ?
						1 :
						0;
			}
			if (diff1 != 0) {
				printf("Write-only test with msgId = %d failed at %s: %d!\n",
				       messagesTbl[i].msgId, __FILE__,
				       __LINE__);
				if (pMV2_master->MV_type == intMV) {
					printf("diff.iVal = %d\n",
					       (pMV2_master->Val.iVal -
						pMV2_slave->Val.iVal));
				} else {
					printf("diff.fVal = %f\n",
					       (pMV2_master->Val.fVal -
						pMV2_slave->Val.fVal));
				}

				exit(-1);
			} else {
				printf("Write-only test with msgId = %d passed!\n\n",
				       messagesTbl[i].msgId);
			}
		}
	}


}


/**
 * @brief fullCircuitTest
 * @param MsgId	message Id to be tested
 * @param val value for transfer
 */
static void fullCircuitTest(uint8_t msgId, union MV_Val val, union MV_Val val2)
{
	tMV *pMV = NULL;
	for (size_t i = 0; i < MV_ARRAY_LENGTH; i++) {
		if (Master_MV_array[i].LD_ID == msgId) {
			pMV = &Master_MV_array[i];
			break;
		}
	}
	/* pMV points at the first MV in case of 2 MVs in the message */

	const opentThermMsg_t *pt = GetMessageTblEntry(msgId);
	if (pt == NULL) {
		printf("Test failed at %s: %d!\n", __FILE__, __LINE__);
		exit(-1);
	}
	/* do we need process 2 MVs ? */
	uint32_t numMVs = (pt->msgDataType2 == none) ? 1u : 2u;

	if (pMV->MV_type == intMV) {
		pMV->Val.iVal = val.iVal;
	} else {
		pMV->Val.fVal = val.fVal;
	}

	openThermResult_t masterRes;
	masterRes = OPENTHERM_WriteSlave(pMV, Master_CommFun);
	if (masterRes != OPENTHERM_ResOK) {
		printf("Test failed at %s: %d!\n", __FILE__, __LINE__);
		printf("Error code %d\n", masterRes);
		exit(-1);
	}

	if (pMV->MV_type == intMV) {
		pMV->Val.iVal = 0;
	} else {
		pMV->Val.fVal = 0.0f;
	}

	openThermResult_t slaveRes;
	slaveRes = OPENTHERM_ReadSlave(pMV, Master_CommFun);
	if (slaveRes != OPENTHERM_ResOK) {
		printf("Test failed at %s: %d!\n", __FILE__, __LINE__);
		printf("Error code %d\n", slaveRes);
		exit(-1);
	}

	int diff = 0;

	if (pMV->MV_type == intMV) {
		diff = (pMV->Val.iVal != val.iVal) ? 1 : 0;
	} else {
		diff = (fabs((double)pMV->Val.fVal - (double)val.fVal) > 0.05) ?
			       1 :
			       0;
	}

	if (diff != 0) {
		printf("Full circuit test with msgId = %d failed at %s: %d!\n",
		       msgId, __FILE__, __LINE__);
		if (pMV->MV_type == intMV) {
			printf("diff.iVal = %d\n", (pMV->Val.iVal - val.iVal));
		} else {
			printf("diff.fVal = %f\n", (pMV->Val.fVal - val.fVal));
		}

		exit(-1);
	} else {
		printf("Full circuit test with msgId = %d passed!\n\n", msgId);
	}
}

/**
 * @brief PrintMVInfo
 * @param pMV pointer to the MV to be printed out
 */
void PrintMVInfo(tMV *const pMV)
{
	if (pMV == NULL) {
		return;
	}
	printf("MV with LD_ID = %d\n", pMV->LD_ID);

	printf("\t Controllable = %d\n", pMV->Ctrl);
	printf("\t Timestamp = %d.%d\n", pMV->TimeStamp.Seconds,
	       pMV->TimeStamp.mSeconds);
}

static void init_MVArrays(void)
{
	printf("%s\n", ">>>>>>>>> init_MVArrays started...");

	if (OPENTHERM_InitMV(pMaster_MV_array, MV_ARRAY_LENGTH) != 0) {
		printf("Test failed at %s: %d!\n", __FILE__, __LINE__);
		exit(-1);
	}

	if (OPENTHERM_InitMV(pSlave_MV_array, MV_ARRAY_LENGTH) != 0) {
		printf("Test failed at %s: %d!\n", __FILE__, __LINE__);
		exit(-1);
	}
}

/**
 * @brief GetSlaveMV
 * @param LdId
 * @return
 */
static tMV *GetSlaveMV(uint8_t LdId)
{
	for (size_t i = 0; i < MV_ARRAY_LENGTH; i++) {
		if (Slave_MV_array[i].LD_ID == LdId) {
			return &Slave_MV_array[i];
		}
	}
	return NULL;
}

/**
 * @brief Master_CommFun
 * @param message
 * @return
 */

static commState cstate = commStateIdle;
static uint32_t wire1; /* master -> slave */

static uint32_t Master_CommFun(uint32_t message)
{
	switch (cstate) {
	case commStateIdle:
		/* the message is to be sent to slave */
		wire1 = message;
		cstate = commStateMasterWaitsForReply;
		/* here begins the slave side */

		openThermResult_t slaveRes = OPENTHERM_SlaveRespond(
			wire1, Master_CommFun, GetSlaveMV);
		if (slaveRes != OPENTHERM_ResOK) {
			printf("Test failed at %s: %d!\n", __FILE__, __LINE__);
			printf("Error code %d\n", slaveRes);
			exit(-1);
		}

		if (cstate == commStateRelpyRcvd) {
			cstate = commStateIdle;
		}
		break;
	case commStateMasterWaitsForReply:
		/* control reaces this point only if slave called commFun for reply*/
		wire1 = message;
		cstate = commStateRelpyRcvd;
		break;
	default:
		break;
	}
	return wire1;
}

/**
 * @brief checkMVs cheks MV array size and init correctness
 */
static void checkMVs(void)
{
	return;
}
