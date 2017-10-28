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
		if (strcmp(tokens[0], "stop") == 0) {
			Udp_sendResponse("Program terminating.\n");
			stopUdpThread();
		}
	}

	free(tokens);
	tokens = NULL;
}
