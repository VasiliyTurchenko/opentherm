/**
  ******************************************************************************
  * @file    hex_gen.c
  * @author  TVV
  * @version V1.0.0
  * @date    1-Nov-2018
  * @brief   Functions for bin to hex conversion
  ******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include "hex_gen.h"

#define ADDR_FIELD	3
#define DATA_FIELD	9
#define DATA_LEN	(16 * 2)
#define CS_FIELD	41
#define TAIL_FIELD	43

/* local functions */
static void reverse(char s[]);

/* exported functions */
/**
 * @brief mybtol
 * @param b low nibble to be converted to char
 * @return char (ASCII)
 */
char mybtol(char b)
{
	b = b & 0x0F;
	return ((b <= 9) ? (b + 0x30) : (b + 0x37));
}
// end of mybtol


/**
 * @brief cton char (an ASCII) to nibble
 * @param b
 * @return byte with b in low nibble
 */
char cton(char b)
{
	if ((b <= 0x39) && (b >= 0x30)) {
		b = b - 0x30;
	} else if ((b <= 0x46) && (b >= 0x41)) {
		b = b - 0x37;
	} else
		b = 0;
	return b;
}
// end of cton


// function generates asciiz string dst

int HexGen16(const char *src, unsigned short int addr, char len, char *dst)
// max 16 consecutive bytes to be converted
{
	char *af, *aff;
	const char *SOL = ":LLADDR00";
	strcpy(dst, SOL);
	af = dst + 1;
	*af = mybtol((len) >> 4);
	af++;
	*af = mybtol(len);

	const char *FIN = "CS\n\0";
	strcpy((dst + CS_FIELD), FIN);

	unsigned short int tmpaddr;
	char i;

	af = dst + ADDR_FIELD + 3; // ptr to the end of the address field

	for (i = 0; i < 4; i++) {
		tmpaddr = addr;
		tmpaddr = tmpaddr >> (i * 4);
		*af = mybtol((char)tmpaddr);
		af--;
	}
	// initialize destination substring
	af = dst + DATA_FIELD;
	aff = af + DATA_LEN;
	while (af <= aff) {
		*af = 0x30;
		af++;
	}
	// convert
	af = dst + 9;
	aff = af + (len * sizeof(len) * 2);
	while (af <= aff) {
		*af = mybtol((*src) >> 4);
		af++;
		*af = mybtol(*src);
		af++;
		src++;
	}
	// checksum calculation
	af = dst + 1;
	aff = dst + CS_FIELD - 1;
	uint8_t checksum = 0;

	while (af <= aff) {
		checksum = checksum + ((cton(*af) << 4) | (cton(*(af + 1))));
		af++;
		af++;
	}
	checksum = (~checksum) + 1;
	*af = (char)mybtol(checksum >> 4);
	af++;
	*af = (char)mybtol(checksum);

	return 0;
}
// end of HexGen16


/**
 * @brief HexGenEOF
 * @param dst
 * @return
 */
int HexGenEOF(char *dst) // generates EOF record type
{
	const char *sEOF = ":00000001FF\n\0";
	strcpy(dst, sEOF);
	return 0;
}

/**
 * @brief uint32_to_asciiz uint32_t value to ascii string
 * @param num value to be converted
 * @param result_string is a pointer to the string of the enough length to hold up to 10 symbols
 * @retval none
 */
void uint32_to_asciiz(uint32_t num, char *result_string)
{
	size_t i;
	for (i = 0U; i < 10U; i++) {
		result_string[9u -i] = (char)((num % 10U) + 0x30U);
		num /= 10U;
	}
}

/**
  * @brief	int32_to_asciiz converts int32_t value to asciiz string
  * @param	num value to be converted
  * @param	result_string pointer to the string of the enough length to hold up to 11 symbols
  * @retval	none
  */
void int32_to_asciiz(int32_t num, char *result_string)
{
	int32_t i;
	int32_t sign;
	sign = num;
	i = 0;
	do {
		result_string[i++] = (char)(abs(num % 10)) + '0';
	} while (num /= 10);
	if (sign < 0)
		result_string[i++] = '-';
	result_string[i] = '\0';
	reverse(result_string);
}

/**
 * @brief reverse K&R function
 * @param s
 */
void reverse(char s[])
{
	char c;
	size_t j = strlen(s) - 1u;
	size_t i;
	for (i = 0; i < j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

/**
  * @brief	uint16_to_asciiz converts uint16_t value to asciiz string
  * @note
  * @param	num value to be converted
  * @param	result_string pointer to the string of the enough length to hold up to 5 symbols
  * @retval	none
  */
void uint16_to_asciiz(uint16_t num, char *result_string)
{
	uint8_t i;
	for (i = 0U; i < 5U; i++) {
		result_string[4U - i] = (char)((num % 10U) + 0x30U);
		num /= 10U;
	}
}

/**
  * @brief	uint8_to_asciiz converts uint8_t value to asciiz string
  * @note
  * @param	num value to be converted
  * @param	result_string pointer to the string of the enough length to hold up to 3 symbols
  * @retval	none
  */
void uint8_to_asciiz(uint8_t num, char *result_string)
{
	uint8_t i;
	for (i = 0U; i < 3U; i++) {
		result_string[2U - i] = (char)((num % 10U) + 0x30U);
		num /= 10U;
	}
}

/***************************** END OF FILE *****************************/
