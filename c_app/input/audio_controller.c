#include "../input/audio_controller.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdio.h>

#include "../audio/audio_mixer.h"
#include "../audio/beat_maker.h"
#include "../devices/joy_stick.h"
#include "../devices/accelerometer.h"
#include "../lib/util.h"

#define DECREASE -5
#define INCREASE 5

#define X_TRIGGER_THRESHOLD 20000
#define Y_TRIGGER_THRESHOLD 20000
#define Z_TRIGGER_THRESHOLD 31000

#define POLL_TIME 10000000 // 10 ms
#define JOYSTICK_CENTER_DEBOUNCE_TIME 600000000 // 600 ms
#define JOYSTICK_UDLR_DEBOUNCE_TIME 100000000 // 100 ms
#define ACCELEROMETER_DEBOUNCE_TIME 200000000 // 200 ms

static pthread_t audioControllerThreadId;
static _Bool stopping = false;
static void* audioControllerThread(void* arg);

static void changeVolume(int change);
static void changeTempo(int change);
static void changeBeat(void);

void AudioController_init(void)
{
	JoyStick_init();
	Accelerometer_init();
	pthread_create(&audioControllerThreadId, NULL, audioControllerThread, NULL);
}

void AudioController_cleanup(void)
{
	printf("Stopping audio controller thread...\n");

	stopping = true;
	pthread_join(audioControllerThreadId, NULL);

	printf("Done...\n");
	fflush(stdout);
}

static void changeVolume(int change)
{
	int currentVolume = AudioMixer_getVolume();
	AudioMixer_setVolume(currentVolume + change);
}

static void changeTempo(int change)
{
	int currentTempo = BeatMaker_getTempo();
	BeatMaker_setTempo(currentTempo + change);
}

static void changeBeat(void)
{
	mode_t currentMode = BeatMaker_getMode();
	if (currentMode == NONE) {
		BeatMaker_setMode(STANDARD_ROCK);
	}
	else if (currentMode == STANDARD_ROCK) {
		BeatMaker_setMode(DEATH_METAL);
	}
	else if (currentMode == DEATH_METAL) {
		BeatMaker_setMode(NONE);
	}
}

static void* audioControllerThread(void* arg)
{
	while (!stopping) {
		//printf("hey\n");

		// joy stick events
		joyStickState_t state = JoyStick_getState();
		if (state.JOY_STICK_UP == 0) {
			changeVolume(INCREASE);
			Util_sleep(0, JOYSTICK_UDLR_DEBOUNCE_TIME);
			continue;
		}
		else if (state.JOY_STICK_DOWN == 0) {
			changeVolume(DECREASE);
			Util_sleep(0, JOYSTICK_UDLR_DEBOUNCE_TIME);
			continue;
		}
		else if (state.JOY_STICK_RIGHT == 0) {
			changeTempo(INCREASE);
			Util_sleep(0, JOYSTICK_UDLR_DEBOUNCE_TIME);
			continue;
		}
		else if (state.JOY_STICK_LEFT == 0) {
			changeTempo(DECREASE);
			Util_sleep(0, JOYSTICK_UDLR_DEBOUNCE_TIME);
			continue;
		}
		else if (state.JOY_STICK_CENTER == 0) {
			changeBeat();
			Util_sleep(0, JOYSTICK_CENTER_DEBOUNCE_TIME);
			continue;
		}

		// accelerometer events

		xyz_t xyzVals = Accelerometer_getXYZVals();
		int16_t xAccel = abs(xyzVals.x);
		int16_t yAccel = abs(xyzVals.y);
		int16_t zAccel = abs(xyzVals.z);

		// X
		if (xAccel > X_TRIGGER_THRESHOLD) {
			printf("x motion\n");
			BeatMaker_playSnare();
			Util_sleep(0, ACCELEROMETER_DEBOUNCE_TIME);
			continue;
		}

		// Y
		if (yAccel > Y_TRIGGER_THRESHOLD) {
			printf("y motion\n");
			BeatMaker_playHiHatClosed();
			Util_sleep(0, ACCELEROMETER_DEBOUNCE_TIME);
			continue;
		}

		// Z
		if (zAccel > Z_TRIGGER_THRESHOLD) {
			printf("z motion\n");
			BeatMaker_playBass();
			Util_sleep(0, ACCELEROMETER_DEBOUNCE_TIME);
			continue;
		}

		Util_sleep(0, POLL_TIME);
	}

	return NULL;
}
