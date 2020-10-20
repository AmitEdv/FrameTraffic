#include "TestUnitGenerator.h"
#include <time.h>
#include <WTypesBase.h>
#include "TestUnitUtils.h"
#include "TimeSample.h"
#include "Generator.h"

timeSampleInfo_t mGeneratingStartTime;

static void onFrameGenerated(uint8_t* pFrame, uint16_t frameSize); 
 
void testGeneratorUnit()
{
	//TEST HERE: the random generating
	//comment-in "generateFrames" and send the desired amount of frames to generate
	//in order to run this test.
	//==================================
	/*
	setOnFrameGeneratedCB(onFrameGenerated);
	sampleTime(&mGeneratingStartTime);

	//send as argument, the amount of frames you'd like to generate
	generateFrames(2);
	*/


	//TEST HERE: A custom frame input to check reads and writes of fields.
	//comment-in the desired test case or create your own.
	//comment-in the required action in "TEST ACTIONS" in order to run it.
	//==================================
	
	//TEST INPUTS
	//-----------
	//Test case 1. result expected: id = 0x300, dataLen = 7, data = all 0s
	//uint8_t frame[FRAME_TOTAL_MAX_SIZE_bytes] = { 0xB0, 0x0E, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 
	
	//Test case 2. result expected : id = 0x200, dataLen = 8, data = all 0s
	uint8_t frame[FRAME_TOTAL_MAX_SIZE_bytes] = { 0xA0, 0x0F, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};


	//TEST ACTIONS
	//-----------

	printf("Before writing data: \n");
	printBufferInHex(frame, FRAME_TOTAL_MAX_SIZE_bytes);
	uint8_t data[DATA_FIELD_MAX_SIZE_bytes] = { 0x11, 0x22, 0x33, 0x44 };
	uint8_t dataLen = 4;
	writeDataValues(frame, FRAME_TOTAL_MAX_SIZE_bytes, data, dataLen);
	printf("After writing data: \n");
	printBufferInHex(frame, FRAME_TOTAL_MAX_SIZE_bytes);

	//onFrameGenerated((uint8_t*)&frame, FRAME_TOTAL_MAX_SIZE_bytes);
	



}

static void onFrameGenerated(uint8_t* pFrame, uint16_t frameSize)
{
	timeSampleInfo_t generatingEndTime;
	sampleTime(&generatingEndTime);

	uint16_t timeDiff = getTimeDifferenceInMillis(&mGeneratingStartTime, &generatingEndTime);
	printf("Test Unit: Generating start sec = %d , millis = %d \n", mGeneratingStartTime.epochSeconds, mGeneratingStartTime.milliseconds);
	printf("Test Unit: Generating end   sec = %d , millis = %d \n", generatingEndTime.epochSeconds, generatingEndTime.milliseconds);
	printf("Test Unit: Generating rate = %d \n", timeDiff);
	if (timeDiff > FRAME_GENERATE_INTERVAL_MAX_millis || timeDiff < FRAME_GENERATE_INTERVAL_MIN_millis)
	{
		printf("Test Unit: onFrameGenerated, invalid generating rate!");
	}
	sampleTime(&mGeneratingStartTime);

	if (frameSize < FRAME_TOTAL_MIN_SIZE_bytes || frameSize > FRAME_TOTAL_MAX_SIZE_bytes)
	{
		printf("Test Unit: onFrameGenerated, invalid frame size! size = %d \n", frameSize);
	}
	printf("Test Unit: onFrameGenerated, frame size = %d, frame = \n", frameSize);

	uint16_t id = readIdentifier(pFrame, frameSize);
	printf("Test Unit: onFrameGenerated, read id = 0x%0x \n", id);

	uint8_t dataSize = readDLC(pFrame, frameSize);
	printf("Test Unit: onFrameGenerated, read data size = %d \n", dataSize);
	uint16_t dataSizeOverride = 0x06;
	if (writeDLC(pFrame, frameSize, dataSizeOverride) != WRITE_SUCCESS)
	{
		printf("Test Unit: onFrameGenerated, write data size Failed! \n");
	}
	printf("Test Unit: onFrameGenerated, write data size = %d \n", dataSizeOverride);
	dataSize = (uint8_t)readDLC(pFrame, frameSize);
	printf("Test Unit: onFrameGenerated, read data size = %d \n", dataSize);

	printBufferInHex(pFrame, frameSize);
	printBufferInBinary(pFrame, frameSize);
}
