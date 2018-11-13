/**
  ******************************************************************************
  * @file    hex_gen.c
  * @author  TVV
  * @version V1.0.0
  * @date    1-Nov-2018
  * @brief   Functions for bin to hex conversion - Header file of hex_gen.c
  ******************************************************************************/

#ifndef __HEX_GEN
#define __HEX_GEN

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Exported functions --------------------------------------------------------

int HexGen16(const char *src, unsigned short int addr, char len,
	     char *dst);  // max 16 consecutive bytes to be converted
int HexGenEOF(char *dst); // generates EOF record type

char mybtol(char b);      // 09-12-2017
void uint32_to_asciiz(uint32_t num, char *result_string); // 09-12-2017
void uint16_to_asciiz(uint16_t num, char *result_string); // 09-12-2017
void uint8_to_asciiz(uint8_t num, char *result_string);   // 09-12-2017
void int32_to_asciiz(int32_t num, char *result_string);   // 01-04-2018

#ifdef __cplusplus
}
#endif

#endif

/***************************** END OF FILE *************************/
