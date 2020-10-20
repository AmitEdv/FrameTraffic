#include "Generator.h"
#include <stdio.h>
#include <time.h>
#include <windows.h>

typedef struct framePlaceholder
{
	uint8_t placeholder[FRAME_TOTAL_MAX_SIZE_bytes];
} framePlaceholder_t;

onFrameGenerated_cb mOnGeneratedCb = 0;
framePlaceholder_t mFrameBuffer;

static void populateFrame(uint8_t* pFrame, uint16_t frameSize);
static uint16_t generateInterval();
static uint16_t generateIdValue();
static void writeIdentifier(uint8_t* pframe, uint16_t frameSize, uint16_t identifier);

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
		//<AMIT>
		intervalMilliseconds = 50;
		//intervalMilliseconds = generateInterval();
		//</AMIT

		//<AMIT>
		printf("interval = %d \n", intervalMilliseconds);
		//</AMIT>

		//TODO - refactor to threads, so the app could still write into a file, and run, while generating.
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
	//<AMIT>
	printf("id = %0x \n", id);
	//</AMIT>
	writeIdentifier(pFrame, frameSize, id);
}

static uint16_t generateIdValue()
{
	uint16_t id = rand() % 3;
	id++;
	id <<= 8;
	return id;
}

static void writeIdentifier(uint8_t* pframe, uint16_t frameSize, uint16_t identifier)
{
	if (identifier > 0x7FF)
	{
		//identifier field is 11 bits
		return FAILURE_INVALID_ARGS;
	}

	if (frameSize < sizeof(uint16_t))
	{
		return FAILURE_INVALID_ARGS;
	}

	identifier <<= 4;
	uint8_t identifierLSB = (uint8_t)identifier;
	uint8_t identifierMSB = (uint8_t)(identifier >> 8);

	//keep other frame fields values, clean current identifier bits, set value
	uint8_t frameByte0Cpy = *pframe;
	uint8_t frameByte1Cpy = *(pframe + 1);
	identifierLSB = identifierLSB | (frameByte0Cpy & 0x0F);
	identifierMSB = identifierMSB | (frameByte1Cpy & 0x80);

	*pframe = identifierMSB;
	*(pframe + 1) = identifierLSB;

	return identifier;
}

uint16_t readIdentifier(uint8_t const* pframe, uint16_t frameSize)
{
	if (frameSize < sizeof(uint16_t)) 
	{
		return FAILURE_INVALID_ARGS;
	}

	uint8_t frameByte0Cpy = *pframe;
	uint8_t frameByte1Cpy = *(pframe + 1);

	uint16_t result = 0;
	uint8_t* pResultLSB= (uint8_t*)&result;
	uint8_t* pResultMSB = ((uint8_t*)&result) + 1;
	*pResultMSB |= (frameByte0Cpy & 0x7F);
	*pResultLSB |= (frameByte1Cpy & 0xF0);
	result >>= 4;

	return result;
}
