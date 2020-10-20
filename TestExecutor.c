#include <stdio.h>
#include "TestUnitGenerator.h"
#include "TestUnitDetector.h"
#include "Detector.h"

static void run(uint8_t numOfFrames);

int main(int argc, char* argv[])
{
	//Please comment out the program you'd like to launch. One at a time.

	printf("=========Generator Unit============= \n");
	testGeneratorUnit();
	printf("==================================== \n");

	printf("=========Detector Unit============== \n");
	//testDetectorUnit();
	printf("==================================== \n");

	printf("=========Run Program================ \n");
	//run(10);
	printf("==================================== \n");

	return 0;
}

static void run(uint8_t numOfFrames)
{
	setOnFrameGeneratedCB(onReceiveFrame);
	generateFrames(numOfFrames);
}

