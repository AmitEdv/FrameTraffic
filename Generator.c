#include "Generator.h"
#include <stdio.h>
#include <time.h>


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
	uint8_t frameByte0Cpy = *pframe;
	uint8_t frameByte1Cpy = *(pframe + 1);
	identifierLSB = identifierLSB | (frameByte0Cpy & 0x0F);
	identifierMSB = identifierMSB | (frameByte1Cpy & 0x80);

	*pframe = identifierMSB;
	*(pframe + 1) = identifierLSB;
}

uint16_t readIdentifier(uint8_t const* pframe, uint16_t frameSize)
{
	if (frameSize < sizeof(uint16_t)) 
	{
		return ERROR_FIELD_READ;
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

