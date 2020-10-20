#include "TestUnitUtils.h"

#define BITS_IN_BYTES			8

void printBufferInHex(uint8_t* pBuff, uint16_t buffSize)
{
	printf("0x ");
	for (uint16_t i = 0; i < buffSize; i++)
		printf("%02x - ", pBuff[i]);

	printf("\n ");
}

void printBufferInBinary(uint8_t* pBuff, uint16_t buffSize)
{
	printf("( ");
	uint8_t bitVal;
	for (uint16_t i = 0; i < buffSize; i++)
	{
		for (int8_t j = BITS_IN_BYTES - 1; j >= 0; j--) {
			bitVal = (pBuff[i] >> j) & 0x01;
			printf("%u", bitVal);
		}
		printf(" - ");
	}

	printf(")b \n ");
}
