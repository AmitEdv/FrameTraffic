#ifndef _GENERATOR_H
#define _GENERATOR_H

#include <stdint.h>

#define FRAME_TOTAL_MAX_SIZE_bytes				14 // frame size in bits = 108
#define FRAME_TOTAL_MIN_SIZE_bytes				6 // data can vary between 0-8 bytes 

#define DATA_FIELD_MAX_SIZE_bytes				8
#define DATA_FIELD_MIN_SIZE_bytes				0

#define WRITE_FAILURE_INVALID_ARGS				-1
#define WRITE_SUCCESS							0

#define FRAME_GENERATE_INTERVAL_MIN_millis		50
#define FRAME_GENERATE_INTERVAL_MAX_millis		100

typedef void (*onFrameGenerated_cb)(uint8_t* pFrame, uint16_t frameSize);

void setOnFrameGeneratedCB(onFrameGenerated_cb cb);
void generateFrames(int amount);
uint16_t readIdentifier(uint8_t const* pframe, uint16_t frameSize);
uint8_t readDLC(uint8_t const* pframe, uint16_t frameSize);
uint8_t writeDLC(uint8_t* pframe, uint16_t frameSize, uint16_t dlc);

#endif // ! _GENERATOR_H

