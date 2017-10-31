#include "../audio/audio_mixer.h"
#include "../audio/beat_maker.h"
#include "../devices/accelerometer.h"
#include "../lib/util.h"
#include "../network/udp_interface.h"
#include "../network/udp.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

#define BUFFER_LENGTH 1024
#define WHITE_SPACE " \t\r\n\a"

// commands
#define GET_CMD "get"
#define SET_CMD "set"
#define STOP_CMD "stop"
// options
#define MODE_OP "mode"
#define VOLUME_OP "volume"
#define TEMPO_OP "tempo"
// modes
#define DEATH_METAL_MODE "death_metal"
#define STANDARD_ROCK_MODE "standard_rock"
#define NONE_MODE "none"

static pthread_t* p_tid = NULL;
static _Bool stopThread = false;

static void stopUdpThread();
static void* udpThread(void* arg);
static void handleCmd(char* rawInput);

void UdpInterface_start()
{
	if (p_tid != NULL) {
		return;
	}

	stopThread = false;
	p_tid = malloc(sizeof(*p_tid));
	pthread_create(p_tid, NULL, udpThread, NULL);
}

void UdpInterface_waitUntilStop()
{
	pthread_join(*p_tid, NULL);
	free(p_tid);
	p_tid = NULL;
}

static void stopUdpThread()
{
	stopThread = true;
}

static void* udpThread(void* arg)
{
	Udp_initConnection();

	// Buffer to hold packet data
	char messageBuffer[BUFFER_LENGTH];
	while (!stopThread) {
		Udp_getIncomingMessage(messageBuffer, BUFFER_LENGTH);
		handleCmd(messageBuffer);
	}

	Udp_closeConnection();

	pthread_exit(NULL);
}

static void handleCmd(char* cmd)
{
	int numTokens;
	char** tokens = Util_strsplit(cmd, WHITE_SPACE, &numTokens);

	if (numTokens > 0) {
		char* command = tokens[0];
		if (strcmp(command, STOP_CMD) == 0) {
			Udp_sendResponse("Program terminating.\n");
			stopUdpThread();
		}

		if (strcmp(command, GET_CMD) == 0) {
			char* option = tokens[1];
			if (strcmp(option, MODE_OP) == 0) {
				char responseBuffer[BUFFER_LENGTH];

				mode_t beatMode = BeatMaker_getMode();
				switch (beatMode) {
				case NONE:
					sprintf(responseBuffer, NONE_MODE);
					break;
				case STANDARD_ROCK:
					sprintf(responseBuffer, STANDARD_ROCK_MODE);
					break;
				case DEATH_METAL:
					sprintf(responseBuffer, DEATH_METAL_MODE);
					break;
				default:
					break;
				}

				Udp_sendResponse(responseBuffer);
			}

			if (strcmp(option, TEMPO_OP) == 0) {
				char responseBuffer[BUFFER_LENGTH];
				sprintf(responseBuffer, "%d", BeatMaker_getTempo());
				Udp_sendResponse(responseBuffer);
			}

			if (strcmp(option, VOLUME_OP) == 0) {
				char responseBuffer[BUFFER_LENGTH];
				sprintf(responseBuffer, "%d", AudioMixer_getVolume());
				Udp_sendResponse(responseBuffer);
			}
		}

		if (strcmp(command, SET_CMD) == 0) {
			char* option = tokens[1];
			if (strcmp(option, MODE_OP) == 0) {
				char* value = tokens[2];
				if (strcmp(value, NONE_MODE) == 0) {
					BeatMaker_setMode(NONE);
				} else if (strcmp(value, STANDARD_ROCK_MODE) == 0) {
					BeatMaker_setMode(STANDARD_ROCK);
				} else if (strcmp(value, DEATH_METAL_MODE) == 0) {
					BeatMaker_setMode(DEATH_METAL);
				}
			}

			if (strcmp(option, TEMPO_OP) == 0) {
				int value = atoi(tokens[2]);
				if (!BeatMaker_setTempo(value)) {
					char responseBuffer[BUFFER_LENGTH];
					sprintf(responseBuffer,
							"{\"error\": true, \"message\": \"ERROR: Tempo must be between %d and %d.\"}\n",
							BEATMAKER_MIN_TEMPO, BEATMAKER_MAX_TEMPO);
					Udp_sendResponse(responseBuffer);
				}
			}

			if (strcmp(option, VOLUME_OP) == 0) {
				int value = atoi(tokens[2]);
				if (!AudioMixer_setVolume(value)) {
					char responseBuffer[BUFFER_LENGTH];
					sprintf(responseBuffer,
							"{\"error\": true, \"message\": \"ERROR: Volume must be between 0 and %d.\"}\n",
							AUDIOMIXER_MAX_VOLUME);
					Udp_sendResponse(responseBuffer);
				}
			}
		}
	}

	free(tokens);
	tokens = NULL;
}
