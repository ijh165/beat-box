#include "../lib/util.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

void Util_readFileToBuffer(char* fileName, char* buffer, int bufferSize)
{
	FILE* file = fopen(fileName, "r");
	if (file == NULL) {
		printf("ERROR: Unable to open file (%s) for read\n", fileName);
		exit(-1);
	}

	// Read string (line)
	fgets(buffer, bufferSize, file);
	// Close
	fclose(file);
}

void Util_writeToFile(char* fileName, char* strToWrite)
{
	FILE* file = fopen(fileName, "w");
	if (file == NULL) {
		printf("ERROR OPENING %s.", fileName);
		exit(-1);
	}
	if (fputs(strToWrite, file) < 0) {
		printf("ERROR WRITING DATA");
		exit(-1);
	}

	fclose(file);
}

void Util_sleep(long seconds, long nanoseconds)
{
	struct timespec reqDelay = {seconds, nanoseconds};
	nanosleep(&reqDelay, (struct timespec *) NULL);
}


char** Util_strsplit(char* str, const char* delim, int* o_numTokens)
{
	const int MAX_TOKENS = strlen(str);
	char** tokens = malloc(sizeof(*tokens) * MAX_TOKENS);

	int i = 0;
	char* token = strtok(str, delim);
	while (token != NULL && i < MAX_TOKENS)
	{
		tokens[i++] = token;
		token = strtok(NULL, delim);
	}
	*o_numTokens = i;

	return tokens;
}

int Util_readValueFromFile(char* fileName)
{
	FILE *file = fopen(fileName, "r");
	if (file == NULL) {
		printf("ERROR: Unable to open file (%s) for read\n", fileName);
		exit(-1);
	}

	// Read string (line)
	const int max_length = 1024;
	char buff[max_length];
	fgets(buff, max_length, file);

	fclose(file);
	int value = buff[0] - '0';
	return value;
}
