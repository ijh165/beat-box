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
		if (strcmp(command, "stop") == 0) {
			Udp_sendResponse("Program terminating.\n");
			stopUdpThread();
		}

		if (strcmp(command, "set") == 0) {
			char* option = tokens[1];
			if (strcmp(option, "beat_mode") == 0) {
				char* value = tokens[2];
				if (strcmp(value, "none") == 0) {
					BeatMaker_setMode(NONE);
				} else if (strcmp(value, "standard_rock") == 0) {
					BeatMaker_setMode(STANDARD_ROCK);
				} else if (strcmp(value, "death_metal") == 0) {
					BeatMaker_setMode(DEATH_METAL);
				}
			}

			if (strcmp(option, "tempo") == 0) {
				int value = atoi(tokens[2]);
				if (!BeatMaker_setTempo(value)) {
					char responseBuffer[BUFFER_LENGTH];
					sprintf(responseBuffer, "ERROR: Tempo must be between %d and %d.\n",
							BEATMAKER_MIN_TEMPO, BEATMAKER_MAX_TEMPO);
					Udp_sendResponse(responseBuffer);
				}
			}

			if (strcmp(option, "volume") == 0) {
				int value = atoi(tokens[2]);
				if (!AudioMixer_setVolume(value)) {
					char responseBuffer[BUFFER_LENGTH];
					sprintf(responseBuffer, "ERROR: Volume must be between 0 and %d.\n", AUDIOMIXER_MAX_VOLUME);
					Udp_sendResponse(responseBuffer);
				}
			}
		}
	}

	free(tokens);
	tokens = NULL;
}
