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
	//<AMIT>
	/*
	printf("getTimeDifferenceInMillis start sec = %d , millis = %d \n", startTimeSample->epochSeconds, startTimeSample->milliseconds);
	printf("getTimeDifferenceInMillis end   sec = %d , millis = %d \n", endTimeSample->epochSeconds, endTimeSample->milliseconds);
	printf("secDiff = %d \n", secDiff);
	printf("res = %d \n", (MILLIS_IN_SEC - startTimeSample->milliseconds) + (secDiff - 1) * MILLIS_IN_SEC + endTimeSample->milliseconds);
	*/
	//</AMIT>

	uint16_t secDiff = (uint16_t)(endTimeSample->epochSeconds - startTimeSample->epochSeconds);
	if (secDiff == 0)
	{
		return (endTimeSample->milliseconds - startTimeSample->milliseconds);
	}

	return (MILLIS_IN_SEC - startTimeSample->milliseconds)
		+ (secDiff - 1) * MILLIS_IN_SEC
		+ endTimeSample->milliseconds;
}