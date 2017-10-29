#include <stdbool.h>
#include <pthread.h>
#include "joy_stick.h";
#include "../lib/util.h";
#include "audio_mixer.h";
#include "beat_maker.h";

#define JOY_STICK_CENTER_FILE "/sys/class/gpio/gpio27/value"
#define JOY_STICK_RIGHT_FILE "/sys/class/gpio/gpio47/value"
#define JOY_STICK_LEFT_FILE "/sys/class/gpio/gpio65/value"
#define JOY_STICK_UP_FILE "/sys/class/gpio/gpio26/value"
#define JOY_STICK_DOWN_FILE "/sys/class/gpio/gpio46/value"

static pthread_t joyStickThreadId;
static void* joyStickThread(void* arg);

void JoyStick_init()
{
	Util_writeToFile(GPIO_EXPORT, "47");
	Util_writeToFile(GPIO_EXPORT, "65");
	Util_writeToFile(GPIO_EXPORT, "26");
	Util_writeToFile(GPIO_EXPORT, "46");
	Util_writeToFile(GPIO_EXPORT, "27");
	pthread_create(&joyStickThreadId, NULL, joyStickThread, NULL);
}

void JoyStick_changeVolume(int change)
{
	int currentVolume = AudioMixer_getVolume();
	AudioMixer_setVolume(currentVolume + change);
}

void JoyStick_changeTempo(int change)
{

}

static void* joyStickThread(void* arg)
{
	joystick joyStick;

	while(true) {
		joyStick.JOY_STICK_CENTER = Util_readValueFromFile(JOY_STICK_CENTER_FILE);
		joyStick.JOY_STICK_RIGHT = Util_readValueFromFile(JOY_STICK_RIGHT_FILE);
		joyStick.JOY_STICK_LEFT = Util_readValueFromFile(JOY_STICK_LEFT_FILE);
		joyStick.JOY_STICK_DOWN = Util_readValueFromFile(JOY_STICK_DOWN_FILE);
		joyStick.JOY_STICK_UP = Util_readValueFromFile(JOY_STICK_UP_FILE);

		if (joyStick.JOY_STICK_UP == 0) {
			JoyStick_changeVolume(5);

		} else if (joyStick.JOY_STICK_DOWN == 0) {
			JoyStick_changeVolume(-5);
		} else if (joyStick.JOY_STICK_LEFT == 0) {


		} else if (joyStick.JOY_STICK_RIGHT == 0) {

		}

		Util_sleep(0,100);
	}
}

