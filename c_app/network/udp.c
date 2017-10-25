#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include "udp.h"

#define PORT 12345

typedef struct sockaddr_in sin_t;

static int socketDescriptor = 0;
static sin_t* p_sin = NULL;

void Udp_initConnection(void)
{
	// Address
	p_sin = (sin_t*) malloc(sizeof(*p_sin));
	memset(p_sin, 0, sizeof(*p_sin));
	p_sin->sin_family = AF_INET;                   // Connection may be from network
	p_sin->sin_addr.s_addr = htonl(INADDR_ANY);    // Host to Network long
	p_sin->sin_port = htons(PORT);                 // Host to Network short

	// Create the socket for UDP
	socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);

	// Bind the socket to the port (PORT) that we specify
	bind (socketDescriptor, (struct sockaddr*) p_sin, sizeof(*p_sin));
}

void Udp_closeConnection(void)
{
	// Close
	close(socketDescriptor);

	//free any dynamic memory
	free(p_sin);
	p_sin = NULL;
}

void Udp_getIncomingMessage(char* buffer, int bufferLength)
{
	// Get the data (blocking)
	// Will change sin (the address) to be the address of the client.
	// Note: sin passes information in and out of call!
	unsigned int sin_len = sizeof(*p_sin);
	int bytesRx = recvfrom(socketDescriptor, buffer, bufferLength, 0,
			(struct sockaddr *) p_sin, &sin_len);

	//printf("HELLOOOOO\n");

	// Make it null terminated (so string functions work):
	// NOTE: Unsafe in some cases; why? only safe when bytesRx < bufferLength
	if (bytesRx < bufferLength) {
		buffer[bytesRx] = 0;
	} else {
		buffer[bufferLength-1] = 0;
	}

	//printf("Message received (%d bytes): \n\n'%s'\n", bytesRx, buffer);
}

void Udp_sendResponse(const char* response)
{
	// Transmit a reply:
	unsigned int sin_len = sizeof(*p_sin);
	sendto(socketDescriptor,
		response, strlen(response),
		0,
		(struct sockaddr *) p_sin, sin_len);
}

