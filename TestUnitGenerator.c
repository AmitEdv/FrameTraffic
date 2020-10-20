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
	setOnFrameGeneratedCB(onFrameGenerated);
	sampleTime(&mGeneratingStartTime);
	generateFrames(2);
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
	printBufferInHex(pFrame, frameSize);
}
