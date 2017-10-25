#include "../devices/pot.h"
#include "../lib/defn.h"
#include "../lib/util.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define A2D_FILE_VOLTAGE0 "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"
#define PIECEWISE_NUM_POINTS 10

static const int PIECEWISE_ARR_SIZE[] = {1, 20, 60, 120, 250, 300, 500, 800, 1200, 2100};
static const int PIECEWISE_POT[] = {0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4100};

static int getVoltage0Reading();
static int getPieceWiseArray(int reading);
static int computePieceWise(int s, float a, float m, float b, float n);

void Pot_init(void)
{
	Util_writeToFile(SLOTS, "BB-ADC");
	while(access(A2D_FILE_VOLTAGE0, R_OK) == -1); // wait until file is ready to be read
}

// Use POT on hardware to determine the voltage, estimate the array size with piece-wise function
int Pot_getArraySize(void)
{
	int reading = getVoltage0Reading();
	return getPieceWiseArray(reading);
}


static int getVoltage0Reading()
{
	const int BUFF_LENGTH = 1024;
	char buff[BUFF_LENGTH];
	Util_readFileToBuffer(A2D_FILE_VOLTAGE0, buff, BUFF_LENGTH);

	int reading = atoi(buff);
	//printf("raw voltage0 reading: %d\n", reading);
	return reading;
}

static int getPieceWiseArray(int reading)
{
	float a = 0;
	float m = 0;
	float b = 0;
	float n = 0;

	if (reading <= 0) {
		return PIECEWISE_ARR_SIZE[0];
	} else if (reading >= 4100) {
		return PIECEWISE_ARR_SIZE[9];
	} else {
		for (int i = 0; i < PIECEWISE_NUM_POINTS; i++) {
			int voltage = PIECEWISE_POT[i];

			if (reading > voltage) {
				a = PIECEWISE_POT[i];
				m = PIECEWISE_ARR_SIZE[i];
			} else {
				b = PIECEWISE_POT[i];
				n = PIECEWISE_ARR_SIZE[i];
				break;
			}
		}
	}
	//printf("reading:%d a:%f b:%f m:%f n:%f \n\n", reading, a, b, m, n);
	return computePieceWise(reading, a, b, m, n);
}

static int computePieceWise(int s,
		float a, float b,
		float m, float n
)
{
	float result = ((s - a)/(b - a)) * (n - m) + m;
	return (int)result;
}
