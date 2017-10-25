#include "devices/display.h"
#include "devices/accelerometer.h"
#include "network/udp_interface.h"

static void init(void);

int main()
{
	init();
	UdpInterface_start();
	UdpInterface_waitUntilStop();

	return 0;
}

static void init(void)
{
	Accelerometer_init();
}

