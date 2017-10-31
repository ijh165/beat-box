#include "../devices/accelerometer.h"
#include "../lib/common.h"
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

void Accelerometer_init()
{
	Util_writeToFile(SLOTS, "BB-I2C1");
	i2cFileDesc = I2c_initI2cBus(I2CDRV_LINUX_BUS1, DEVICE_ADDRESS);
	I2c_writeI2cReg(i2cFileDesc, CTRL_REG1, 0x01);
}

xyz_t Accelerometer_getXYZVals()
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
	xyz_t vals;
	vals.x = x;
	vals.y = y;
	vals.z = z;
	return vals;
}
