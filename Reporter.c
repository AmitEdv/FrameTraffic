#include "Reporter.h"
#include <stdio.h>

void onFrameDetected(frameWrapper_t frame)
{
	//<AMIT>
	printf("onFrameDetected \n");
	//</AMIT>

	//open file
	FILE* fp;
	chmod("./results.csv", 0644);
	errno_t err = fopen_s(&fp, "./results.csv", "a");
	if (fp == 0)
	{
		//TODO - handle failure
		//<AMIT>
		printf("fp = 0 , err = %d \n", err);
		//</AMIT
		return;
	}

	fprintf(fp, "%d,", frame.receivedTime.epochSeconds);
	fprintf(fp, "0x");
	for (int i = 0; i < frame.size; i++)
	{
		fprintf(fp, "%02x,", frame.frameRawContent[i]);
	}
	
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