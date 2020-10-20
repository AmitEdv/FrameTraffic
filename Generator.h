#ifndef _GENERATOR_H
#define _GENERATOR_H

#include <stdint.h>

#define FRAME_ARBITRATION_IDENTIFIER_OFFSET_bits	1
#define FRAME_ARBITRATION_IDENTIFIER_SIZE_bits		11

#define FRAME_CONTROL_DLC_OFFSET_bits				15
#define FRAME_CONTROL_DLC_SIZE_bits					4

#define FRAME_DATA_FIELD_OFFSET_bits				19
#define FRAME_DATA_FIELD_SIZE_bytes					8

#define FRAME_TOTAL_MAX_SIZE_bytes				14 // frame size in bits = 108
#define FRAME_TOTAL_MIN_SIZE_bytes				6 // data can vary between 0-8 bytes 

#define FAILURE_INVALID_ARGS						-1

#define FRAME_GENERATE_INTERVAL_MIN_millis			50
#define FRAME_GENERATE_INTERVAL_MAX_millis			100

typedef void (*onFrameGenerated_cb)(uint8_t* pFrame, uint16_t frameSize);

void setOnFrameGeneratedCB(onFrameGenerated_cb cb);
void generateFrames(int amount);
uint16_t readIdentifier(uint8_t const* pframe, uint16_t frameSize);

#endif // ! _GENERATOR_H

