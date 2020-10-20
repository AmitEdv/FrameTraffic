#include "Detector.h"
#include <string.h>
#include "Generator.h"
#include "Reporter.h"

//AMIT
#include <stdio.h>

#define VALID_RATE_MIN						100

frameWrapper_t mPreviousFrames[AMOUNT_OF_IDS] = {0, };

static uint8_t getPreviousFrameIndexById(uint16_t id);

void onReceiveFrame(uint8_t const* pFrame, uint16_t frameSize)
{
	//<AMIT>
	printf("frame received! \n");
	//</AMIT>

	uint16_t frameId = readIdentifier(pFrame, frameSize);
	if (frameId == WRITE_FAILURE_INVALID_ARGS
		|| (frameId != ALLOWED_ID1
			&& frameId != ALLOWED_ID2
			&& frameId != ALLOWED_ID3))
	{
		//TODO- handle scenario
		return;
	}
	uint8_t dataLen = readDLC(pFrame, frameSize);
	if (dataLen > DATA_FIELD_MAX_SIZE_bytes
		|| dataLen < DATA_FIELD_MIN_SIZE_bytes)
	{
		//TODO- handle scenario
		return;
	}


	frameWrapper_t currFrameWrapper;
	currFrameWrapper.id = frameId;
	currFrameWrapper.dataLength;
	if (frameSize > FRAME_RAW_CONTENT_MAX_SIZE_bytes)
	{
		frameSize = FRAME_RAW_CONTENT_MAX_SIZE_bytes;
	}
	memcpy(&(currFrameWrapper.frameRawContent), pFrame, frameSize);
	currFrameWrapper.size = frameSize;
	sampleTime(&(currFrameWrapper.receivedTime));

	uint8_t previousFrameIndex = getPreviousFrameIndexById(frameId);
	if (previousFrameIndex == -1)
	{
		//TODO- handle scenario
		return;
	}

	currFrameWrapper.validation = validationCheck(&currFrameWrapper, mPreviousFrames + previousFrameIndex);
	mPreviousFrames[previousFrameIndex] = currFrameWrapper;
	onFrameDetected(currFrameWrapper);
}

static uint8_t getPreviousFrameIndexById(uint16_t id)
{
	switch (id)
	{
	case ALLOWED_ID1:
		return 0;
		break;
	case ALLOWED_ID2:
		return 1;
		break;
	case ALLOWED_ID3:
		return 2;
		break;
	default:
		return -1;
	}
}

invalidFrameReason_e validationCheck(frameWrapper_t* curr, frameWrapper_t* prev)
{
	if (prev->id == 0)
	{
		//no previous frame of this id
		return VALID;
	}
	
	if (! isRateValid(curr, prev))
	{
		return INVALID_RATE;
	}

	if (! isDataLengthDifferent(curr, prev))
	{
		return INVALID_FIELD_LENGTH;
	}

	if (! isDataDifferent(curr, prev))
	{
		return INVALID_DATA;
	}

	return VALID;
}

bool isRateValid(frameWrapper_t* curr, frameWrapper_t* prev)
{
	return (getTimeDifferenceInMillis(&(prev->receivedTime), &(curr->receivedTime)) > VALID_RATE_MIN);
}

bool isDataLengthDifferent(frameWrapper_t* curr, frameWrapper_t* prev)
{
	return (curr->dataLength != prev->dataLength);
}

bool isDataDifferent(frameWrapper_t* curr, frameWrapper_t* prev)
{
	//AMIT
	return true;
}
