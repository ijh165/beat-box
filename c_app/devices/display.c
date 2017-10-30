#include "../devices/display.h"
#include "../lib/i2c.h"
#include "../lib/util.h"
#include "../lib/common.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define GPIO_PIN_61_DIRECTION "/sys/class/gpio/gpio61/direction"
#define GPIO_PIN_44_DIRECTION "/sys/class/gpio/gpio44/direction"
#define GPIO_PIN_61_VAL "/sys/class/gpio/gpio61/value"
#define GPIO_PIN_44_VAL "/sys/class/gpio/gpio44/value"

#define DISPLAY_DEVICE_ADDRESS 0x20
#define REG_DIRA 0x00
#define REG_DIRB 0x01
#define REG_OUTA 0x14
#define REG_OUTB 0x15

typedef struct
{
	unsigned char a;
	unsigned char b;
} regVals_t;

// 14-seg display for 0 - 9
static const regVals_t REG_VALS_LUT[] = {
		{0xA1, 0x86}, //0
		{0x80, 0x02}, //1
		{0x31, 0x0E}, //2
		{0xB0, 0x0E}, //3
		{0x90, 0x8A}, //4
		{0xB0, 0x8D}, //5
		{0xB1, 0x8D}, //6
		{0x80, 0x06}, //7
		{0xB1, 0x8E}, //8
		{0xB0, 0x8E} //9
};

static int i2cFileDesc = 0;
static long long num = 0;

static pthread_t* p_tid = NULL;
static _Bool stopThread = false;

static void* displayThread(void* arg);
static void writeDigit(int num);

void Display_init(void)
{
	Util_writeToFile(SLOTS, "BB-I2C1");
	Util_writeToFile(GPIO_EXPORT, "61");
	Util_writeToFile(GPIO_EXPORT, "44");
	Util_writeToFile(GPIO_PIN_61_DIRECTION, "out");
	Util_writeToFile(GPIO_PIN_44_DIRECTION, "out");

	i2cFileDesc = I2c_initI2cBus(I2CDRV_LINUX_BUS1, DISPLAY_DEVICE_ADDRESS);

	//printf("i2cFileDesc: %d\n", i2cFileDesc);

	I2c_writeI2cReg(i2cFileDesc, REG_DIRA, 0x00);
	I2c_writeI2cReg(i2cFileDesc, REG_DIRB, 0x00);
}

void Display_displayDigit(long long argNum)
{
	if (p_tid == NULL || stopThread) {
		stopThread = false;
		p_tid = malloc(sizeof(*p_tid));
		pthread_create(p_tid, NULL, displayThread, NULL);
	}

	num = argNum;
}

void Display_turnOffDisplay()
{
	stopThread = true;
	pthread_join(*p_tid, NULL);
	free(p_tid);
	p_tid = NULL;
}

static void* displayThread(void* arg)
{
	int rightDigit = 0;
	int leftDigit = 0;

	while(!stopThread) {
		// Calculate the digit to display
		if (num > 99) {
			rightDigit = 9;
			leftDigit = 9;
		} else if (num < 10) {
			rightDigit = num%10;
			leftDigit = 0;
		} else {
			rightDigit = num%10;
			leftDigit = (num/10) % 10;
		}

		// Flicker

		Util_writeToFile(GPIO_PIN_61_VAL, "0");
		Util_writeToFile(GPIO_PIN_44_VAL, "0");
		writeDigit(leftDigit);
		Util_writeToFile(GPIO_PIN_61_VAL, "1");
		Util_sleep(0, 5000000);

		Util_writeToFile(GPIO_PIN_61_VAL, "0");
		Util_writeToFile(GPIO_PIN_44_VAL, "0");
		writeDigit(rightDigit);
		Util_writeToFile(GPIO_PIN_44_VAL, "1");
		Util_sleep(0, 5000000);
	}

	Util_writeToFile(GPIO_PIN_61_VAL, "0");
	Util_writeToFile(GPIO_PIN_44_VAL, "0");

	pthread_exit(NULL);
}

static void writeDigit(int num) {
	const regVals_t* regVals = &REG_VALS_LUT[num];
	I2c_writeI2cReg(i2cFileDesc, REG_OUTA, regVals->a);
	I2c_writeI2cReg(i2cFileDesc, REG_OUTB, regVals->b);
}
