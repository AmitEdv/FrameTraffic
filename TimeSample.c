#include "TimeSample.h"
#include <time.h>
#include <WTypesBase.h>

#define MILLIS_IN_SEC			1000

void sampleTime(timeSampleInfo_t* timeSample)
{
	timeSample->epochSeconds = getTimeInSeconds();
	timeSample->milliseconds = getTimeInMilliseconds();
}

uint16_t getTimeInMilliseconds()
{
	// struct to request the time
	SYSTEMTIME oSytemTime;
	//buffer for the output
	wchar_t szTime[1000];

	// getting the system time (Greenwich Mean Time); GetLocalTime () is possible too

	GetSystemTime(&oSytemTime);
	return oSytemTime.wMilliseconds;
}

long getTimeInSeconds()
{
	return time(0);
}

uint16_t getTimeDifferenceInMillis(timeSampleInfo_t* startTimeSample, timeSampleInfo_t* endTimeSample)
{	
	uint16_t msDiff;
	uint16_t secDiff = (uint16_t)(endTimeSample->epochSeconds - startTimeSample->epochSeconds);
	if (secDiff == 0)
	{
		msDiff = (endTimeSample->milliseconds - startTimeSample->milliseconds);
	}
	else
	{
		msDiff = (MILLIS_IN_SEC - startTimeSample->milliseconds)
			+ (secDiff - 1) * MILLIS_IN_SEC
			+ endTimeSample->milliseconds;

	}

	printf("getTimeDifferenceInMillis: diff = %d ms \n", msDiff);
	return msDiff;
}