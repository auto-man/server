#include "socket.h"

Socket::Socket()
{
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(9999);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
}

bool Socket::init()
{
	lisfd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == lisfd){
		cout<<"SOCKET INIT FAILED IN SOCKET()"<<endl;
		return false;
	}

	if(-1 == bind(lisfd,(sockaddr*)&servaddr,sizeof(servaddr))){
		cout<<"SOCKET INIT FAILED IN BIND()"<<endl;
		return false;
	}

	if(-1 == listen(lisfd,1000)){
		cout<<"SOCKET INIT FAILED IN LISTEN()"<<endl;
		return false;
	}

	return true;
}

int Socket::getLisfd()
{
	return lisfd;
}

sockaddr_in& Socket::getStruct()
{
	return servaddr;
}

Socket::~Socket()
{
	cout<<"SOCKET DESTROY NOW"<<endl;
}
