#ifndef UTIL_H
#define UTIL_H

void Util_readFileToBuffer(char* fileName, char* buffer, int bufferSize);
void Util_writeToFile(char* fileName, char* strToWrite);
void Util_sleep(long seconds, long nanoseconds);
char** Util_strsplit(char* str, const char* delim, int* o_numTokens);

#endif
