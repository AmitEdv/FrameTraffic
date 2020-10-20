#include "Generator.h"
#include <stdio.h>
#include <time.h>
#include <windows.h>

#define FRAME_ARBITRATION_IDENTIFIER_END_BYTE		2

#define FRAME_CONTROL_DLC_START_BYTE				1
#define FRAME_CONTROL_DLC_END_BYTE					2

#define FRAME_DATA_FIELD_START_BYTE					2
#define FRAME_DATA_FIELD_END_BYTE					10
#define FRAME_DATA_FIELD_FIRST_BIT_OFFSET			3 //zero based, within the first byte

#define BYTE_MAX_VAL								0xFF
#define BITS_IN_BYTE								8

typedef struct framePlaceholder
{
	uint8_t placeholder[FRAME_TOTAL_MAX_SIZE_bytes];
} framePlaceholder_t;

onFrameGenerated_cb mOnGeneratedCb = 0;
framePlaceholder_t mFrameBuffer;

static void populateFrame(uint8_t* pFrame, uint16_t frameSize);
static uint16_t generateInterval();
static uint16_t generateIdValue();
static uint16_t generateDLCValue();
static void generateDataValues(uint8_t* pDataBuffer, uint16_t frameSize, uint8_t dataLen);
static uint16_t writeIdentifier(uint8_t* pframe, uint16_t frameSize, uint16_t identifier);

void setOnFrameGeneratedCB(onFrameGenerated_cb cb)
{
	//TODO - if more than 1 cb is required, modify to hold multiple callbacks
	mOnGeneratedCb = cb;
}

void generateFrames(int amount)
{
	srand((unsigned int)time(0));

	uint16_t intervalMilliseconds;
	for (int i = 0; i < amount; i++)
	{
		intervalMilliseconds = generateInterval();

		//TODO - refactor to threads
		Sleep(intervalMilliseconds);

		populateFrame(&(mFrameBuffer.placeholder), sizeof(framePlaceholder_t));
		if (mOnGeneratedCb != 0)
		{
			mOnGeneratedCb(&(mFrameBuffer.placeholder), sizeof(framePlaceholder_t));
		}
	}

}

static uint16_t generateInterval()
{
	return (rand() % (FRAME_GENERATE_INTERVAL_MAX_millis - FRAME_GENERATE_INTERVAL_MIN_millis + 1)) 
		+ FRAME_GENERATE_INTERVAL_MIN_millis;
}

static void populateFrame(uint8_t* pFrame, uint16_t frameSize)
{
	memset(pFrame, 0xFF, frameSize);
	
	uint16_t id = generateIdValue();
	writeIdentifier(pFrame, frameSize, id);
	uint8_t dlc = (uint16_t)generateDLCValue();
	writeDLC(pFrame, frameSize, dlc);

	uint8_t data[DATA_FIELD_MAX_SIZE_bytes];
	generateDataValues(&data, DATA_FIELD_MAX_SIZE_bytes, dlc);
	writeDataValues(pFrame, frameSize, &data, dlc);
}

static uint16_t generateIdValue()
{
	uint16_t id = rand() % 3;
	id++;
	id <<= 8;
	return id;
}

static uint16_t generateDLCValue()
{
	return (rand() % (DATA_FIELD_MAX_SIZE_bytes - DATA_FIELD_MIN_SIZE_bytes + 1))
		+ DATA_FIELD_MIN_SIZE_bytes;
}

static uint16_t writeIdentifier(uint8_t* pframe, uint16_t frameSize, uint16_t identifier)
{
	if (identifier > 0x7FF)
	{
		//identifier field is 11 bits
		return WRITE_FAILURE_INVALID_ARGS;
	}

	if (frameSize < FRAME_ARBITRATION_IDENTIFIER_END_BYTE + 1)
	{
		return WRITE_FAILURE_INVALID_ARGS;
	}

	identifier <<= 4;
	uint8_t identifierLSB = (uint8_t)identifier;
	uint8_t identifierMSB = (uint8_t)(identifier >> 8);

	//keep other frame fields values, clean current identifier bits, set value
	uint8_t frameByteLSBCpy = *pframe;
	uint8_t frameByteMSBCpy = *(pframe + 1);
	identifierLSB = identifierLSB | (frameByteLSBCpy & 0x0F);
	identifierMSB = identifierMSB | (frameByteMSBCpy & 0x80);

	*pframe = identifierMSB;
	*(pframe + 1) = identifierLSB;

	return identifier;
}

uint16_t readIdentifier(uint8_t const* pframe, uint16_t frameSize)
{
	if (frameSize < FRAME_ARBITRATION_IDENTIFIER_END_BYTE + 1)
	{
		return READ_FAILURE_INVALID_ARGS;
	}

	uint8_t frameMSBByteCpy = *pframe;
	uint8_t frameLSBByteCpy = *(pframe + 1);

	uint16_t result = 0;
	uint8_t* pResultLSB= (uint8_t*)&result;
	uint8_t* pResultMSB = ((uint8_t*)&result) + 1;
	*pResultMSB |= (frameMSBByteCpy & 0x7F);
	*pResultLSB |= (frameLSBByteCpy & 0xF0);
	result >>= 4;

	return result;
}

uint8_t writeDLC(uint8_t* pframe, uint16_t frameSize, uint16_t dlc)
{
	if (dlc > DATA_FIELD_MAX_SIZE_bytes || dlc < DATA_FIELD_MIN_SIZE_bytes)
	{
		return WRITE_FAILURE_INVALID_ARGS;
	}

	if (frameSize < FRAME_CONTROL_DLC_END_BYTE + 1)
	{
		return WRITE_FAILURE_INVALID_ARGS;
	}

	dlc <<= 5;
	uint8_t dlcLSB = (uint8_t)dlc;
	uint8_t dlcMSB = (uint8_t)(dlc >> 8);

	//keep other frame fields values, clean current identifier bits, set value
	uint8_t frameByteMSBCpy = *(pframe + FRAME_CONTROL_DLC_START_BYTE);
	uint8_t frameByteLSBCpy = *(pframe + FRAME_CONTROL_DLC_START_BYTE + 1);

	dlcLSB = dlcLSB | (frameByteLSBCpy & 0x1F);
	dlcMSB = dlcMSB | (frameByteMSBCpy & 0xFE);

	*(pframe + FRAME_CONTROL_DLC_START_BYTE) = dlcMSB;
	*(pframe + FRAME_CONTROL_DLC_START_BYTE + 1) = dlcLSB;

	return WRITE_SUCCESS;
}

uint16_t readDLC(uint8_t const* pframe, uint16_t frameSize)
{
	if (frameSize < FRAME_CONTROL_DLC_END_BYTE + 1)
	{
		return READ_FAILURE_INVALID_ARGS;
	}

	uint8_t frameMSBByteCpy = *(pframe + FRAME_CONTROL_DLC_START_BYTE);
	uint8_t frameLSBByteCpy = *(pframe + FRAME_CONTROL_DLC_START_BYTE + 1);

	uint16_t result = 0;
	uint8_t* pResultLSB = (uint8_t*)&result;
	uint8_t* pResultMSB = ((uint8_t*)&result) + 1;
	*pResultMSB |= (frameMSBByteCpy & 0x01);
	*pResultLSB |= (frameLSBByteCpy & 0xE0);
	result >>= 5;

	return result;
}

static void generateDataValues(uint8_t* pDataBuffer, uint16_t buffSize, uint8_t dataLen)
{
	if (buffSize < dataLen)
	{
		//TODO - handle scenario
		return;
	}
	
	for (int i = 0; i < dataLen; i++)
	{
		pDataBuffer[i] = rand() % BYTE_MAX_VAL;
	}
}

//NOTE! This function modifies data argument content !
uint8_t writeDataValues(uint8_t* pFrame, uint16_t frameSize, uint8_t* pData, uint8_t dataLen)
{
	if (frameSize < dataLen)
	{
		return WRITE_FAILURE_INVALID_ARGS;
	}

	if (dataLen == 0)
	{
		return 0;
	}

	uint16_t totalBitsToWrite = dataLen * BITS_IN_BYTE;

	//handle start byte of data in frame
	uint8_t* pStartDataByteInFrame = pFrame + FRAME_DATA_FIELD_START_BYTE;
	//store other fields values on the same byte
	uint8_t otherFieldsValues = *(pStartDataByteInFrame) & 0xE0;
	
	//<AMIT>
	printf("WRITE original start byte = 0x%0x \n", *pStartDataByteInFrame);
	printf("WRITE totalBitsToWrite = %d \n", totalBitsToWrite);
	//</AMIT>

	//extract bits that complete the byte of frame, where the data field starts
	uint8_t dataFirstBitsToCompleteByte = (*pData & 0xF8) >> FRAME_DATA_FIELD_FIRST_BIT_OFFSET;

	//set in frame
	*pStartDataByteInFrame = otherFieldsValues | dataFirstBitsToCompleteByte;

	totalBitsToWrite -= (BITS_IN_BYTE - FRAME_DATA_FIELD_FIRST_BIT_OFFSET);

	//handle frame bytes of data-field only (no other fields on those bytes)
	uint8_t* pCurrByteInFrame = pStartDataByteInFrame + 1; // +1 becuase we already handled first byte.
	while (totalBitsToWrite > BITS_IN_BYTE)
	{
		//skip used bits, and copy rest of byte content into frame
		*(pData) <<= (BITS_IN_BYTE - FRAME_DATA_FIELD_FIRST_BIT_OFFSET); 
		memcpy(pCurrByteInFrame, pData, 1);

		//complete the byte in frame with following bits from data
		dataFirstBitsToCompleteByte = (*(pData + 1) & 0xF8) >> FRAME_DATA_FIELD_FIRST_BIT_OFFSET;
		*pCurrByteInFrame |= dataFirstBitsToCompleteByte;

		pCurrByteInFrame++;
		pData++;
		totalBitsToWrite -= BITS_IN_BYTE;
		
		//<AMIT>
		printf("WRITE totalBitsToWrite = %d \n", totalBitsToWrite);
		//</AMIT>
	}


	//handle end byte of data in frame
	*(pData) <<= (BITS_IN_BYTE - FRAME_DATA_FIELD_FIRST_BIT_OFFSET);
	uint8_t* pEndDataByteInFrame = pCurrByteInFrame;
	//store other fields values on the same byte
	otherFieldsValues = *(pEndDataByteInFrame) & 0x1F;
	*pEndDataByteInFrame = (otherFieldsValues | *pData);

	//<AMIT>
	printf("WRITE last bytes original valus = 0x%0x \n", *pEndDataByteInFrame);
	printf("WRITE otherFieldsValues = 0x%0x \n", otherFieldsValues);
	printf("WRITE *pData = 0x%0x \n", *pData);
	//</AMIT>



	return dataLen;
}