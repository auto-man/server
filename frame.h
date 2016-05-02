#ifndef __FRAME__H
#define __FRAME__H

#include "common.h"
#include "socket.h"
#include "pthreadpool.h"

class frame
{
public:
	frame();
	bool main();
	~frame();
private:
	Socket m_Socket;
	pthreadpool m_pthreadpool;
};

#endif
