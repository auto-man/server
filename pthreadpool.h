#ifndef __PTHREADPOOL__H
#define __PTHREADPOOL__H

#include "common.h"
#include "socket.h"
#include <pthread.h>
#include <signal.h>

struct thread
{
	pthread_t tid;
	int	fd;
	volatile sig_atomic_t status;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	thread* next;
};

static volatile sig_atomic_t STATUS = true;

class pthreadpool
{
public:
	pthreadpool();
	bool init(int);
	bool wake(int);
	//bool sleep();
	bool destroy();
	static void* func(void*);
	static void signalSocketClose(int);
	~pthreadpool();
private:
	thread* m_headRun;
	//thread* m_headStop;
	pthread_attr_t	m_attr;
	sched_param	m_param;
	static const int RUN = 1;
	static const int STOP = 0;
	static const int HOST = -1;
};

#endif
