#include <stdbool.h>
#include <pthread.h>
#include <stdio.h>
#include "audio_controller.h"
#include "audio_mixer.h"
#include "beat_maker.h"
#include "../devices/joy_stick.h"

#define DECREASE -5
#define INCREASE 5

static pthread_t audioControllerThreadId;
static _Bool stopping = false;
static void* audioControllerThread(void* arg);

static void AudioController_changeVolume(int change);
static void AudioController_changeTempo(int change);
static void AudioController_changeBeat(void);

void AudioController_init(void)
{
	JoyStick_init();
	pthread_create(&audioControllerThreadId, NULL, audioControllerThread, NULL);
}

void AudioController_cleanup(void)
{
	JoyStick_cleanup();

	printf("Stopping audio controller thread...\n");

	stopping = true;
	pthread_join(audioControllerThreadId, NULL);

	printf("Done...\n");
	fflush(stdout);
}

static void AudioController_changeVolume(int change)
{
	int currentVolume = AudioMixer_getVolume();
	AudioMixer_setVolume(currentVolume + change);
}

static void AudioController_changeTempo(int change)
{
	int currentTempo = BeatMaker_getTempo();
	BeatMaker_setTempo(currentTempo + change);
}

static void AudioController_changeBeat(void)
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
	while(!stopping) {
		if (joyStick.JOY_STICK_UP == 0) {
			AudioController_changeVolume(INCREASE);
		}
		else if (joyStick.JOY_STICK_DOWN == 0) {
			AudioController_changeVolume(DECREASE);
		}
		else if (joyStick.JOY_STICK_RIGHT == 0) {
			AudioController_changeTempo(INCREASE);
		}
		else if (joyStick.JOY_STICK_LEFT == 0) {
			AudioController_changeTempo(DECREASE);
		}
		else if (joyStick.JOY_STICK_CENTER == 0) {
			AudioController_changeBeat();
		}
	}

	return NULL;
}


