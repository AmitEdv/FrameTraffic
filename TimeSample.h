#ifndef _TIMESAMPLE_H_
#define _TIMESAMPLE_H_

#include <stdint.h>

#define FAILURE_MORE_THAN_A_SEC_PASSED	-1

typedef struct timeSampleInfo
{
	long epochSeconds;
	uint16_t milliseconds;
} timeSampleInfo_t;

void sampleTime(timeSampleInfo_t* timeSample);
uint16_t getTimeInMilliseconds();
long getTimeInSeconds();
uint16_t getTimeDifferenceInMillis(timeSampleInfo_t* startTimeSample, timeSampleInfo_t* endTimeSample);


#endif // !_TIMESAMPLE_H_

