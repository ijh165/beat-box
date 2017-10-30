#include "../audio/beat_maker.h"
#include "../audio/audio_mixer.h"
#include "../lib/util.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define SPLASH "beatbox-wav-files/100060__menegass__gui-drum-splash-hard.wav"
#define HI_HAT_CLOSED "beatbox-wav-files/100054__menegass__gui-drum-ch.wav"
#define HI_HAT_OPENED "beatbox-wav-files/100055__menegass__gui-drum-co.wav"
#define SNARE "beatbox-wav-files/100058__menegass__gui-drum-snare-hard.wav"
#define BASS "beatbox-wav-files/100051__menegass__gui-drum-bd-hard.wav"

#define DEFAULT_BPM 120
#define HALF_BEATS_PER_CYCLE 8
#define QUARTER_BEATS_PER_CYCLE 16

#define ONE_OVER_N_BEAT_TIME_NS(bpm, n) ( ((60 * 1.0) / (bpm) / (n)) * 1000000000 )
#define HALF_BEAT_TIME_NS(bpm) (ONE_OVER_N_BEAT_TIME_NS( (bpm), 2 ))
#define QUARTER_BEAT_TIME_NS(bpm) (ONE_OVER_N_BEAT_TIME_NS( (bpm), 4 ))

//sound samples
wavedata_t splashSample;
wavedata_t hiHatClosedSample;
wavedata_t hiHatOpenedSample;
wavedata_t snareSample;
wavedata_t bassSample;

typedef struct {
	_Bool splash;
	_Bool hiHatClosed;
	_Bool hiHatOpened;
	_Bool snare;
	_Bool bass;
} soundsToPlay_t;

const soundsToPlay_t standardRock[HALF_BEATS_PER_CYCLE] = {
	{0, 1, 0, 0, 1},
	{0, 1, 0, 0, 0},
	{0, 1, 0, 1, 0},
	{0, 1, 0, 0, 0},
	{0, 1, 0, 0, 1},
	{0, 1, 0, 0, 0},
	{0, 1, 0, 1, 0},
	{0, 1, 0, 0, 0}
};

const soundsToPlay_t deathMetal[QUARTER_BEATS_PER_CYCLE] = {
	{1, 0, 0, 0, 1},
	{0, 0, 0, 0, 1},
	{0, 0, 1, 1, 1},
	{0, 0, 0, 0, 1},
	{0, 0, 1, 0, 1},
	{0, 0, 0, 0, 1},
	{0, 0, 1, 1, 1},
	{0, 0, 0, 0, 1},
	{0, 0, 1, 0, 1},
	{0, 0, 0, 0, 1},
	{0, 0, 1, 1, 1},
	{0, 0, 0, 0, 1},
	{0, 0, 1, 0, 1},
	{0, 0, 0, 0, 1},
	{0, 0, 1, 1, 1},
	{0, 0, 0, 0, 1}
};

mode_t beatMode;
uint16_t tempo;

static _Bool stopThread = false;
static pthread_t drumBeatThreadId;
//static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void* drumBeatThread(void* arg);
static void playSounds(soundsToPlay_t sounds);

void BeatMaker_init()
{
	AudioMixer_init();
	AudioMixer_readWaveFileIntoMemory(SPLASH, &splashSample);
	AudioMixer_readWaveFileIntoMemory(HI_HAT_CLOSED, &hiHatClosedSample);
	AudioMixer_readWaveFileIntoMemory(HI_HAT_OPENED, &hiHatOpenedSample);
	AudioMixer_readWaveFileIntoMemory(SNARE, &snareSample);
	AudioMixer_readWaveFileIntoMemory(BASS, &bassSample);

	BeatMaker_setMode(NONE);
	BeatMaker_setTempo(DEFAULT_BPM);

	pthread_create(&drumBeatThreadId, NULL, drumBeatThread, NULL);
}

void BeatMaker_setMode(mode_t newBeatMode)
{
	beatMode = newBeatMode;
	//printf("beatmode changed!\n");
}

_Bool BeatMaker_setTempo(int newTempo)
{
	if (newTempo < BEATMAKER_MIN_TEMPO || newTempo > BEATMAKER_MAX_TEMPO) {
		printf("ERROR: Tempo must be between %d and %d.\n",
			BEATMAKER_MIN_TEMPO, BEATMAKER_MAX_TEMPO);
		return false;
	}
	tempo = newTempo;
	return true;
}

mode_t BeatMaker_getMode()
{
	return beatMode;
}

int BeatMaker_getTempo()
{
	return tempo;
}

void BeatMaker_playBass()
{
	AudioMixer_queueSound(&bassSample);
}

void BeatMaker_playSnare()
{
	AudioMixer_queueSound(&snareSample);
}

void BeatMaker_playHiHatClosed()
{
	AudioMixer_queueSound(&hiHatClosedSample);
}

void BeatMaker_cleanup()
{
	// Stop the PCM generation thread
	stopThread = true;
	pthread_join(drumBeatThreadId, NULL);

	AudioMixer_cleanup();
	AudioMixer_freeWaveFileData(&splashSample);
	AudioMixer_freeWaveFileData(&hiHatClosedSample);
	AudioMixer_freeWaveFileData(&hiHatOpenedSample);
	AudioMixer_freeWaveFileData(&snareSample);
	AudioMixer_freeWaveFileData(&bassSample);
}

static void* drumBeatThread(void* arg)
{
	while (!stopThread)
	{
		if (beatMode == NONE) {
			continue;
		}

		// standard rock
		for (int i = 0;
			i < HALF_BEATS_PER_CYCLE && !stopThread && beatMode == STANDARD_ROCK;
			i++) {
			const soundsToPlay_t sounds = standardRock[i];
			playSounds(sounds);
			Util_sleep(0, HALF_BEAT_TIME_NS(tempo));
		}

		// death metal
		for (int i = 0;
			i < QUARTER_BEATS_PER_CYCLE && !stopThread && beatMode == DEATH_METAL;
			i++) {
			const soundsToPlay_t sounds = deathMetal[i];
			playSounds(sounds);
			Util_sleep(0, QUARTER_BEAT_TIME_NS(tempo));
		}
	}

	pthread_exit(NULL);
}

static void playSounds(soundsToPlay_t sounds)
{
	_Bool splash = sounds.splash;
	_Bool hiHatClosed = sounds.hiHatClosed;
	_Bool hiHatOpened = sounds.hiHatOpened;
	_Bool snare = sounds.snare;
	_Bool bass = sounds.bass;

	if (splash) {
		AudioMixer_queueSound(&splashSample);
	}

	if (hiHatClosed) {
		AudioMixer_queueSound(&hiHatClosedSample);
	}

	if (hiHatOpened) {
		AudioMixer_queueSound(&hiHatOpenedSample);
	}

	if (snare) {
		AudioMixer_queueSound(&snareSample);
	}

	if (bass) {
		AudioMixer_queueSound(&bassSample);
	}
}
