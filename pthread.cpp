#include "pthreadpool.h"

pthreadpool::pthreadpool()
{
	m_headRun = new thread;
	m_headRun->tid	= pthread_self();
	m_headRun->fd	= -1;
	m_headRun->epoll_fd = -1;
	m_headRun->status = HOST;
	m_headRun->next	= NULL;
	pthread_attr_init(&m_attr);
	m_param.sched_priority = 0;
	sched_setscheduler(getpid(),2,&m_param);
	pthread_attr_setschedparam(&m_attr,&m_param);
	pthread_attr_setstacksize(&m_attr,512*1024);
	STATUS = true;
/*
	m_headStop = new thread;
	m_headStop->tid	= 0;
	m_headStop->fd	= -1;
	m_headStop->status = HOST;
	m_headStop->next= NULL;
*/
}

bool pthreadpool::init(int number)
{
	thread* BUFF = m_headRun;
	for (int i = 0;i != number;++i){
		thread* buff = new thread;
		buff->fd = -1;
		buff->status = STOP;
		buff->epollFd = epoll_create(100);
		for (int j = 0;j != 100;++j){
			buff->event[j].events	= EPOLLIN;
			buff->event[j].data.fd	= 0;
		}
		pthread_mutex_init(&buff->mutex,NULL);
		pthread_cond_init(&buff->cond,NULL);
		buff->next = NULL;
		pthread_create(&buff->tid,&m_attr,func,(void*)buff);
		while(BUFF->next != NULL){
			BUFF = BUFF->next;
		}
		BUFF->next = buff;
	}
	return true;
}

bool pthreadpool::addEpoll(int fd)
{
	thread* BUFF = m_headRun;
	while(BUFF->next->status != RUN && BUFF != NULL){
		BUFF = BUFF->next;
	}
	if(BUFF == NULL){
		this->wake(fd);
		return false;
	}
	
	int i = 0;
	while(BUFF->event[i].data.fd != 0){
		++i;
	}

	BUFF->event[i].data.fd = fd;
	epoll_ctl(BUFF->epollFd,EPOLL_CTL_ADD,fd,&BUFF->event[i]);

	return true;
}

bool pthreadpool::wake(int fd)
{
	thread* BUFF = m_headRun;
	while(BUFF != NULL && BUFF->status != STOP){
		BUFF = BUFF->next;
	}

	if(BUFF == NULL){
		cout<<"WORKER ALL WORKED!"<<endl;
		return false;
	}

	BUFF->fd = fd;
	BUFF->status = RUN;
	pthread_cond_signal(&BUFF->cond);

/*	pthread* sBUFF = m_headStop;
	pthread* rBUFF = m_headRun;
	pthread* BUFF;
	if(sBUFF->next == NULL){
		return false;
	}

	BUFF = sBUFF->next;
	BUFF->fd = fd;
	BUFF->next = NULL;
	sBUFF->next = sBUFF->next->next;
	BUFF->status = RUN;

	if(rBUFF->next == NULL){
		rBUFF->next = BUFF;
		return true;
	}

	BUFF->next = rBUFF->next;
	rBUFF->next = BUFF;
*/
	return true;
}

bool pthreadpool::destroy()
{
	cout<<"DESTROY NOW"<<endl;
	STATUS = false;
	thread* BUFF = m_headRun;
	while(m_headRun->next != NULL){
		BUFF = BUFF->next;
		m_headRun->next = BUFF->next;
		pthread_cond_signal(&BUFF->cond);
		pthread_mutex_destroy(&BUFF->mutex);
		pthread_cond_destroy(&BUFF->cond);
		pthread_join(BUFF->tid,NULL);
		delete BUFF;
		BUFF = m_headRun;
	}
	cout<<"DESTROY SUCCESSED"<<endl;
	return true;
}

void* pthreadpool::func(void* BUFF)
{
	thread* TEMP = (thread*)BUFF;
	int epoll_fd;
	int epoll_temp;
	int epoll_buff;
	int data;
	char buff[512];
	if(signal(SIGPIPE,signalSocketClose) == SIG_ERR){
		cout<<"SIG HANDER FAILED"<<endl;
		exit(0);
	}
	while(STATUS){
		pthread_mutex_lock(&TEMP->mutex);
		if(TEMP->status == STOP){
			pthread_cond_wait(&TEMP->cond,&TEMP->mutex);
		}
		if(STATUS == false){
			pthread_mutex_unlock(&TEMP->mutex);
			cout<<TEMP->tid<<" DESTORYED"<<endl;
			return TEMP;
		}
		cout<<"============GET CLIENT============="<<endl;
		cout<<TEMP->fd<<endl;
		while(TEMP->status){
			epoll_fd = epoll_wait(TEMP->epollFd,TEMP->event,100,0);
			if(epoll_fd != 0){
				for (i = 0;i != epoll_fd;++i){
					epoll_temp = TEMP->event[i].events;
					if(epoll_temp & EPOLLIN){
						//do something
						//if(some one cut the TCP){
						//	epoll_ctl(TEMP->epollFd,EPOLL_CTL_DEL,TEMP->event[i].data.fd,&TEMP->event[i]);
						//	TEMP->event[i].data.fd = 0;
						//}
					}
					else
						continue;
				}
			}
		}
			/*if(send(TEMP->fd,"Hello,you are connected!\n",26,0) == -1){
				cout<<"SEND FAILED"<<endl;
				TEMP->status = STOP;
			}
			if(recv(TEMP->fd,buff,512,0) == -1){
				cout<<"RECV FAILED"<<endl;
				TEMP->status = STOP;
			}
		}*/
		cout<<"SERVER DESTROY"<<endl;
		close(TEMP->fd);
		TEMP->fd	= 0;
		TEMP->status	= STOP;
		pthread_mutex_unlock(&TEMP->mutex);
	}
	return TEMP;
}

void pthreadpool::signalSocketClose(int signo)
{
	return;
}

pthreadpool::~pthreadpool()
{
	pthread_attr_destroy(&m_attr);
	pthread_mutex_destroy(&m_headRun->mutex);
	pthread_cond_destroy(&m_headRun->cond);
}

/*
int main(void)
{
	pthreadpool BUFF;
	BUFF.init(5);
	BUFF.wake(1);
	sleep(1);
	BUFF.destroy();
	return 0;
}*/
