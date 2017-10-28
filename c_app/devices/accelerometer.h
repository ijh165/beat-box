#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <stdint.h>

typedef struct
{
	int16_t x;
	int16_t y;
	int16_t z;
} xyz_t;

void Accelerometer_init();
void Accelerometer_startMotionDetection();
void Accelerometer_stopMotionDetection();

#endif
