#include "../devices/accelerometer.h"
#include "../lib/defn.h"
#include "../lib/i2c.h"
#include "../lib/util.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define DEVICE_ADDRESS 0x1C
#define STATUS 0x00
#define OUT_X_MSB 0x01
#define OUT_X_LSB 0x02
#define OUT_Y_MSB 0x03
#define OUT_Y_LSB 0x04
#define OUT_Z_MSB 0x05
#define OUT_Z_LSB 0x06
#define CTRL_REG1 0x2A

static int i2cFileDesc = 0;

static pthread_t* p_tid = NULL;
static _Bool stopThread = false;

void (*xMotionEvent)(void) = NULL;
void (*yMotionEvent)(void) = NULL;
void (*zMotionEvent)(void) = NULL;

static void* motionDetectionThread(void* arg);
static xyz_t* Accelerometer_getXYZVals();
static _Bool triggerMotionEvent(
	void (*event)(void),
	const int16_t accel,
	const int16_t triggerThreshold,
	const int16_t debounceThreshold,
	_Bool* o_debounced,
	char axis
);

void Accelerometer_init()
{
	Util_writeToFile(SLOTS, "BB-I2C1");
	i2cFileDesc = I2c_initI2cBus(I2CDRV_LINUX_BUS1, DEVICE_ADDRESS);
	I2c_writeI2cReg(i2cFileDesc, CTRL_REG1, 0x01);
}

void Accelerometer_setMotionEvents(
	void (*arg_xMotionEvent)(void),
	void (*arg_yMotionEvent)(void),
	void (*arg_zMotionEvent)(void)
)
{
	xMotionEvent = arg_xMotionEvent;
	yMotionEvent = arg_yMotionEvent;
	zMotionEvent = arg_zMotionEvent;
}

void Accelerometer_startMotionDetection()
{
	stopThread = false;
	p_tid = malloc(sizeof(*p_tid));
	pthread_create(p_tid, NULL, motionDetectionThread, NULL);
}

void Accelerometer_stopMotionDetection()
{
	stopThread = true;
	pthread_join(*p_tid, NULL);
	free(p_tid);
	p_tid = NULL;
}

static void* motionDetectionThread(void* arg)
{
	const int16_t X_TRIGGER_THRESHOLD = 20000;
	const int16_t X_DEBOUNCE_THRESHOLD = 10000;
	const int16_t Y_TRIGGER_THRESHOLD = 15000;
	const int16_t Y_DEBOUNCE_THRESHOLD = 5000;
	const int16_t Z_TRIGGER_THRESHOLD = 30000;
	const int16_t Z_DEBOUNCE_THRESHOLD = 20000;

	_Bool xDebounced = false;
	_Bool yDebounced = false;
	_Bool zDebounced = false;

	while (!stopThread) {
		xyz_t* xyzVals = Accelerometer_getXYZVals();
		_Bool xEventTriggered, yEventTriggered, zEventTriggered;
		xEventTriggered = yEventTriggered = zEventTriggered = false;
		if (!yEventTriggered && !zEventTriggered)
			xEventTriggered = triggerMotionEvent(xMotionEvent, xyzVals->x, X_TRIGGER_THRESHOLD, X_DEBOUNCE_THRESHOLD, &xDebounced, 'X');
		if (!xEventTriggered && !zEventTriggered)
			yEventTriggered = triggerMotionEvent(yMotionEvent, xyzVals->y, Y_TRIGGER_THRESHOLD, Y_DEBOUNCE_THRESHOLD, &yDebounced, 'Y');
		if (!xEventTriggered && !yEventTriggered)
			zEventTriggered = triggerMotionEvent(zMotionEvent, xyzVals->z, Z_TRIGGER_THRESHOLD, Z_DEBOUNCE_THRESHOLD, &zDebounced, 'Z');

		free(xyzVals);
		xyzVals = NULL;
	}
	pthread_exit(NULL);
}

static xyz_t* Accelerometer_getXYZVals()
{
	const size_t numBytes = 7;
	unsigned char bytes[numBytes];
	I2c_readI2cRegMultiBytes(i2cFileDesc, STATUS, numBytes, bytes);

	/*printf("OUT_X_MSB = 0x%02x\n", bytes[OUT_X_MSB]);
	printf("OUT_X_LSB = 0x%02x\n", bytes[OUT_X_LSB]);
	printf("\n");
	printf("OUT_Y_MSB = 0x%02x\n", bytes[OUT_Y_MSB]);
	printf("OUT_Y_LSB = 0x%02x\n", bytes[OUT_Y_LSB]);
	printf("\n");
	printf("OUT_Z_MSB = 0x%02x\n", bytes[OUT_Z_MSB]);
	printf("OUT_Z_LSB = 0x%02x\n", bytes[OUT_Z_LSB]);
	printf("\n");*/

	int16_t x = (bytes[OUT_X_MSB] << 8) | (bytes[OUT_X_LSB]);
	int16_t y = (bytes[OUT_Y_MSB] << 8) | (bytes[OUT_Y_LSB]);
	int16_t z = (bytes[OUT_Z_MSB] << 8) | (bytes[OUT_Z_LSB]);

	//build return struct
	xyz_t* vals = malloc(sizeof(*vals));
	vals->x = x;
	vals->y = y;
	vals->z = z;
	return vals;
}

static _Bool triggerMotionEvent(
	void (*event)(void),
	const int16_t accel,
	const int16_t triggerThreshold,
	const int16_t debounceThreshold,
	_Bool* o_debounced,
	char axis
)
{
	_Bool triggered = false;

	if (accel > triggerThreshold) {
		if (!(*o_debounced)) {
			if(event != NULL)
				event();
			else
				printf("%c-Accel: %d\n", axis, accel);

			triggered = true;
		}

		*o_debounced = true;
	}

	if (accel <= debounceThreshold) {
		*o_debounced = false;
	}

	return triggered;
}
