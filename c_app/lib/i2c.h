#ifndef I2C_H
#define I2C_H

#include <stdlib.h>

#define I2CDRV_LINUX_BUS0 "/dev/i2c-0"
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2CDRV_LINUX_BUS2 "/dev/i2c-2"

int I2c_initI2cBus(char* bus, int address);
void I2c_writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value);
unsigned char I2c_readI2cReg(int i2cFileDesc, unsigned char regAddr);
void I2c_readI2cRegMultiBytes(int i2cFileDesc, unsigned char regAddr, size_t bytes, void* o_value);

#endif
