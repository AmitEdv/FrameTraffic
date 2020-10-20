#include "TestUnitUtils.h"

void printBufferInHex(uint8_t* pBuff, uint16_t buffSize)
{
	printf("0x ");
	for (uint16_t i = 0; i < buffSize; i++)
		printf("%02x - ", pBuff[i]);

	printf("\n ");
}
