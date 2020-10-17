#include "Generator.h"
#include <stdio.h>
#include <time.h>
//<AMIT>
#include <stdio.h>
//</AMIT>

typedef struct framePlaceholder
{
	uint8_t placeholder[FRAME_TOTAL_REQUIRED_SIZE_bytes];
} framePlaceholder_t;

framePlaceholder_t mframeBuffer;

static void populateFrame();
static uint16_t generateIdValue();
static void writeIdentifier(uint8_t* pframe, uint16_t frameSize, uint16_t identifier);
static uint16_t readIdentifier(uint8_t const* pframe, uint16_t frameSize);

void generateFrames(int amount)
{
	populateFrame();

}

uint8_t* getFramePtr()
{
	return &(mframeBuffer.placeholder);
}

static void populateFrame()
{
	memset(&mframeBuffer, 0xFF, sizeof(framePlaceholder_t));
	
	uint16_t id = generateIdValue();
	//<AMIT>
	printf("id = %0x \n", id);
	//</AMIT>
	writeIdentifier(&(mframeBuffer.placeholder), sizeof(framePlaceholder_t), id);
}

static uint16_t generateIdValue()
{
	uint16_t id = time(0) % 3;
	//<AMIT>
	printf("rand = %0x \n", id);
	//</AMIT>
	id++;
	id <<= 8;
	return id;
}

static void writeIdentifier(uint8_t* pframe, uint16_t frameSize, uint16_t identifier)
{
	if (identifier > 0x7FF)
	{
		//identifier field is 11 bits
		return ERROR_FIELD_READ;
	}

	if (frameSize < sizeof(uint16_t))
	{
		return ERROR_FIELD_READ;
	}

	identifier <<= 4;
	uint8_t identifierLSB = identifier;
	uint8_t identifierMSB = identifier >> 8;

	//keep other frame fields values, clean current identifier bits, set value
	uint8_t frameBytes0Cpy = *pframe;
	uint8_t frameBytes1Cpy = *(pframe + 1);
	identifierLSB = identifierLSB | (frameBytes0Cpy & 0x0F);
	identifierMSB = identifierMSB | (frameBytes1Cpy & 0x80);
	
	//<AMIT>
	printf("frameBytes0Cpy = 0x%0x, frameBytes1Cpy = 0x%0x \n", frameBytes0Cpy, frameBytes1Cpy);
	printf("cleared Byte 0 = 0x%0x, cleared Byte 1 = 0x%0x \n", (frameBytes0Cpy & 0x07), (frameBytes1Cpy & 0x80));
	printf("identifierMSB = 0x%0x, identifierLSB = 0x%0x \n", identifierMSB, identifierLSB);
	//</AMIT>

	*pframe = identifierMSB;
	*(pframe + 1) = identifierLSB;
}

static uint16_t readIdentifier(uint8_t const* pframe, uint16_t frameSize)
{
	if (frameSize < sizeof(uint16_t)) 
	{
		return ERROR_FIELD_READ;
	}

	uint16_t identifier = *((uint16_t*)pframe);
	identifier &= 0x7FF0;
	identifier >>= 4;

	return identifier;
}

