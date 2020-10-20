#include "TestUnitDetector.h"
#include <stdio.h>
#include "Reporter.h"

void testDetectorUnit()
{
	frameWrapper_t recentFrame;
	frameWrapper_t lastFrame;

	//Expected: valid
	lastFrame.id = 0x100;
	lastFrame.receivedTime.epochSeconds = 3;
	lastFrame.receivedTime.milliseconds = 200;
	recentFrame.id = 0x100;
	recentFrame.receivedTime.epochSeconds = 5;
	recentFrame.receivedTime.milliseconds = 350;

	//Expected: invalid with code 1 (rate)
	/*lastFrame.id = 0x100;
	lastFrame.receivedTime.epochSeconds = 3;
	lastFrame.receivedTime.milliseconds = 200;
	recentFrame.id = 0x100;
	recentFrame.receivedTime.epochSeconds = 3;
	recentFrame.receivedTime.milliseconds = 210;*/
	
	invalidFrameReason_e validationCode = validationCheck(&recentFrame, &lastFrame);
	printf("validation code = %d \n", validationCode);
}