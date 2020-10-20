#include "Reporter.h"
#include <stdio.h>

#define FILE_PATH	"./results.txt"

void onFrameDetected(frameWrapper_t frame)
{
	//<AMIT>
	printf("onFrameDetected \n");
	//</AMIT>

	FILE* fp;
	errno_t err = fopen_s(&fp, FILE_PATH, "a");
	if (fp == 0)
	{
		//TODO - handle failure
		printf("fp = 0 , err = %d \n", err);
		return;
	}

	fprintf(fp, "%d,", frame.receivedTime.epochSeconds);
	fprintf(fp, "0x");
	for (int i = 0; i < frame.size; i++)
	{
		fprintf(fp, "%02x-", frame.frameRawContent[i]);
	}
	fprintf(fp, ",");

	if (frame.validation == VALID)
	{
		fprintf(fp, "valid,");
	}
	else
	{
		fprintf(fp, "invalid,%d,", frame.validation);
	}

	fclose(fp);
}