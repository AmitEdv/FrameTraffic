#ifndef _GENERATOR_H
#define _GENERATOR_H

#include <stdint.h>

#define FRAME_ARBITRATION_IDENTIFIER_OFFSET_bits	1
#define FRAME_ARBITRATION_IDENTIFIER_SIZE_bits		11

#define FRAME_CONTROL_DLC_OFFSET_bits				15
#define FRAME_CONTROL_DLC_SIZE_bits					4

#define FRAME_DATA_FIELD_OFFSET_bits				19
#define FRAME_DATA_FIELD_SIZE_bytes					8

#define FRAME_TOTAL_REQUIRED_SIZE_bytes				14 // frame size in bits = 108

#define ERROR_FIELD_READ							-1

void generateFrames(int amount);
uint8_t* getFramePtr();

#endif // ! _GENERATOR_H

