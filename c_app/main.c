#include "devices/display.h"
#include "devices/accelerometer.h"
#include "network/udp_interface.h"

#include <stdio.h>
#include <stdbool.h>

static void init(void);

int main()
{
	init();

	Accelerometer_startMotionDetection();
	UdpInterface_start();



	UdpInterface_waitUntilStop();
	Accelerometer_stopMotionDetection();

	return 0;
}

static void init(void)
{

	Accelerometer_init();
}

