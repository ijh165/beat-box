#ifndef UDP_H
#define UDP_H

void Udp_initConnection(void);
void Udp_closeConnection(void);
void Udp_getIncomingMessage(char* buffer, int bufferLength);
void Udp_sendResponse(const char* response);

#endif
