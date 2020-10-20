#ifndef _DETECTOR_H
#define _DETECTOR_H

#include <stdint.h>
#include <stdbool.h>
#include "TimeSample.h"

//<AMIT> - todo, extract to frame api maybe?
#define FRAME_RAW_CONTENT_MAX_SIZE_bytes	14//according to  FRAME_TOTAL_MAX_SIZE_bytes
//</AMIT>

typedef enum invalidFrameReason
{
	VALID,
	INVALID_RATE,
	INVALID_FIELD_LENGTH,
	INVALID_DATA,
} invalidFrameReason_e;

typedef struct frameWrapper
{
	uint16_t id;
	uint16_t size;
	uint8_t dataLength;
	timeSampleInfo_t receivedTime;
	invalidFrameReason_e validation;
	uint8_t frameRawContent[FRAME_RAW_CONTENT_MAX_SIZE_bytes];
} frameWrapper_t;

typedef enum allowedIDs
{
	ALLOWED_ID1 = 0x100,
	ALLOWED_ID2 = 0x200,
	ALLOWED_ID3 = 0x300,
	AMOUNT_OF_IDS = 3,
} allowedIDs_e;

void onReceiveFrame(uint8_t const* pFrame, uint16_t frameSize);
invalidFrameReason_e validationCheck(frameWrapper_t* curr, frameWrapper_t* prev);
bool isRateValid(frameWrapper_t* curr, frameWrapper_t* prev);
bool isDataLengthDifferent(frameWrapper_t* curr, frameWrapper_t* prev);
bool isDataDifferent(frameWrapper_t* curr, frameWrapper_t* prev);

#endif // _DETECTOR_H
