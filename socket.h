#ifndef __SOCKET__H
#define __SOCKET__H

#include "common.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

class Socket
{
public:
	Socket();
	bool init();
	int getLisfd();
	sockaddr_in& getStruct();
	~Socket();
private:
	int lisfd;
	sockaddr_in servaddr;
};

#endif
