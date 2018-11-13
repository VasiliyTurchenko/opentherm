/**
 * Testing routines for hex_gen.c
 * (c) turchenkov@gmail.com
 * 13-Nov-2018
 *
 **/
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "hex_gen.h"

/**
 * @brief diagPrintAndExit
 * @param testname
 * @param line1
 * @param line
 * @param file
 * @param errcode
 */
void __attribute__((noreturn))
diagPrintAndExit(char *testname, int line, char *file, int errcode)
{
	printf("Test %s failed at %s:%d!  Error code = %d\n\n", testname, file,
	       line, (int)errcode);
	exit((int)errcode);
}

static char * ascii = "0123456789ABCDEF";

int main()
{
	printf("hex_gen_test started..\n");

/* char mybtol(char b);*/
	size_t i;
	for (i = 0u; i < 256u; i++) {

		if (i < 15u) {
			if ( mybtol((char)i) != ascii[i] ) {
				diagPrintAndExit("mybtol", __LINE__, __FILE__, (int)i);
			}
			continue;
		}
		if ( mybtol((char)i) != ascii[i%16u] ) {
			diagPrintAndExit("mybtol", __LINE__, __FILE__, (int)i);
			}
	}

	uint32_t num[] = {0x00000000u, 0xFFFFFFFFu, 0x1u};
	char * answers[] = {"0000000000", "4294967295", "0000000001"};
	char test_str[] = {"zzzzzzzzzz"};

	for (size_t i  = 0; i < sizeof (num) / sizeof (num[0]); i++) {
		uint32_to_asciiz(num[i], (char*)&test_str);
		if (strcmp(test_str, answers[i]) != 0) {
			diagPrintAndExit("uint32_to_asciiz", __LINE__, __FILE__, strcmp(test_str, answers[i]));
		}
	}

	int32_t nums[] = {0x00000000, INT32_MIN, INT32_MAX};
	char * answerss[] = {"0", "-2147483648", "2147483647"};
	char test_strs[] = {"zzzzzzzzzzz"};

	for (size_t i  = 0; i < sizeof (nums) / sizeof (nums[0]); i++) {
		int32_to_asciiz(nums[i], (char*)&test_strs);
		if (strcmp(test_strs, answerss[i]) != 0) {
			diagPrintAndExit("int32_to_asciiz", __LINE__, __FILE__, strcmp(test_strs, answerss[i]));
		}
	}

	uint16_t u16nums[] = {0x0000, 0xFFFF, 0x01};
	char * u16ans[] = {"00000", "65535", "00001"};
	char u16test[] = {"zzzzz"};
	for (size_t i  = 0; i < sizeof (u16nums) / sizeof (u16nums[0]); i++) {
		uint16_to_asciiz(u16nums[i], (char*)&u16test);
		if (strcmp(u16test, u16ans[i]) != 0) {
			diagPrintAndExit("uint16_to_asciiz", __LINE__, __FILE__, strcmp(u16test, u16ans[i]));
		}
	}


	uint8_t u8nums[] = {0x00, 255, 1};
	char * u8ans[] = {"000", "255", "001"};
	char u8test[] = {"zzz"};
	for (size_t i  = 0; i < sizeof (u8nums) / sizeof (u8nums[0]); i++) {
		uint8_to_asciiz(u8nums[i], (char*)&u8test);
		if (strcmp(u8test, u8ans[i]) != 0) {
			diagPrintAndExit("uint8_to_asciiz", __LINE__, __FILE__, strcmp(u8test, u8ans[i]));
		}
	}



	printf("hex_gen_test finished..\n");
	return 0;
}

