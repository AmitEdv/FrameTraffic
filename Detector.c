#include "Detector.h"
#include <string.h>
#include <stdio.h>
#include "Generator.h"
#include "Reporter.h"


#define VALID_RATE_MIN						100

frameWrapper_t mPreviousFrames[AMOUNT_OF_IDS] = {0, };

static uint8_t getPreviousFrameIndexById(uint16_t id);

void onReceiveFrame(uint8_t const* pFrame, uint16_t frameSize)
{
	printf("onReceiveFrame \n");

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
	currFrameWrapper.dataLength = dataLen;
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
		printf("validationCheck: INVALID_RATE \n");
		printf("validationCheck: time sample curr = %d sec, %d ms.  prev = %d sec, %d ms \n", 
			curr->receivedTime.epochSeconds, curr->receivedTime.milliseconds, prev->receivedTime.epochSeconds, prev->receivedTime.milliseconds);
		return INVALID_RATE;
	}

	if (! isDataLengthDifferent(curr, prev))
	{
		printf("validationCheck: INVALID_FIELD_LENGTH \n");
		return INVALID_FIELD_LENGTH;
	}

	if (! isDataDifferent(curr, prev))
	{
		printf("validationCheck: INVALID_DATA \n");
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
	//<AMIT>
	printf("validationCheck: data len curr = %d .  data len prev = %d \n", curr->dataLength, prev->dataLength);
	//</AMIT>
	return (curr->dataLength != prev->dataLength);
}

bool isDataDifferent(frameWrapper_t* curr, frameWrapper_t* prev)
{
	//<AMIT>
	/*
	TODO- implement as follows:
	create a boolean array in size of 225. 
	(If we have enough memory we can even store the bytes-array and data in the Heap, to shorten our run-time)
	read data of prev frame.
	Go over the data byte-by-byte. for each byte, 
		sign the array[byteValue] as True.
		example: If I read byte 0x02, I will assign array[0x02] = true;
	read data of prev frame.
	Go over the data byte-by-byte. for each byte,
		if the array[byteValue] has value of True, that means not all bytes are different between frames --> return INVALID
	*/
	//</AMIT>
	return true;
}
