#include "frame.h"

frame::frame(){}

bool frame::main()
{
	int lisfd;
	int confd;

	m_Socket.init();
	lisfd = m_Socket.getLisfd();
	sockaddr_in servaddr = m_Socket.getStruct();

	m_pthreadpool.init(6000);

	while(1){
		if( (confd = accept(lisfd,(sockaddr*)NULL,NULL)) == -1){
			cout<<"ACCEPT FAILED,AND ERROR IN :"<<confd<<endl;
			exit(0);
		}
		if(confd != -1){
			m_pthreadpool.wake(confd);
		}
	}

	close(lisfd);
	m_pthreadpool.destroy();
	return true;
}
	
frame::~frame(){}
