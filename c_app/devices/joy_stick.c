#include <stdbool.h>
#include <pthread.h>
#include <stdio.h>

#include "joy_stick.h"

#include "../lib/common.h"
#include "../lib/util.h"

#define JOY_STICK_CENTER_FILE "/sys/class/gpio/gpio27/value"
#define JOY_STICK_RIGHT_FILE "/sys/class/gpio/gpio47/value"
#define JOY_STICK_LEFT_FILE "/sys/class/gpio/gpio65/value"
#define JOY_STICK_UP_FILE "/sys/class/gpio/gpio26/value"
#define JOY_STICK_DOWN_FILE "/sys/class/gpio/gpio46/value"

#define BUFFER_SIZE 1024



/*static pthread_t joyStickThreadId;
static void* joyStickThread(void* arg);
static _Bool stopping = false;*/

void JoyStick_init(void)
{
	Util_writeToFile(GPIO_EXPORT, "47");
	Util_writeToFile(GPIO_EXPORT, "65");
	Util_writeToFile(GPIO_EXPORT, "26");
	Util_writeToFile(GPIO_EXPORT, "46");
	Util_writeToFile(GPIO_EXPORT, "27");
	//pthread_create(&joyStickThreadId, NULL, joyStickThread, NULL);
}

joyStickState_t JoyStick_getState(void)
{
	joyStickState_t state;

	state.JOY_STICK_CENTER = Util_readValueFromFile(JOY_STICK_CENTER_FILE);
	state.JOY_STICK_RIGHT = Util_readValueFromFile(JOY_STICK_RIGHT_FILE);
	state.JOY_STICK_LEFT = Util_readValueFromFile(JOY_STICK_LEFT_FILE);
	state.JOY_STICK_DOWN = Util_readValueFromFile(JOY_STICK_DOWN_FILE);
	state.JOY_STICK_UP = Util_readValueFromFile(JOY_STICK_UP_FILE);

	return state;
}

/*void JoyStick_cleanup(void)
{
	printf("Stopping joystick thread...\n");

	stopping = true;
	pthread_join(joyStickThreadId, NULL);

	printf("Done...\n");
	fflush(stdout);
}*/

/*static void* joyStickThread(void* arg)
{
	while(!stopping) {
		joyStick.JOY_STICK_CENTER = Util_readValueFromFile(JOY_STICK_CENTER_FILE);
		joyStick.JOY_STICK_RIGHT = Util_readValueFromFile(JOY_STICK_RIGHT_FILE);
		joyStick.JOY_STICK_LEFT = Util_readValueFromFile(JOY_STICK_LEFT_FILE);
		joyStick.JOY_STICK_DOWN = Util_readValueFromFile(JOY_STICK_DOWN_FILE);
		joyStick.JOY_STICK_UP = Util_readValueFromFile(JOY_STICK_UP_FILE);
	}

	pthread_exit(NULL);
}*/

