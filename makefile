
edit : main.o frame.o pthreadpool.o socket.o
	g++ -g -pthread main.o frame.o pthreadpool.o socket.o -o main

main.o : frame.h main.cpp
	g++ -g -c main.cpp
frame.o : common.h pthreadpool.h socket.h frame.cpp
	g++ -g -c -pthread frame.cpp
pthreadpool.o : common.h socket.h pthreadpool.h pthreadpool.cpp
	g++ -g -c -pthread pthreadpool.cpp
socket.o : common.h socket.h socket.cpp
	g++ -g -c socket.cpp

.PHONY : clean

clean : 
	rm -rf *.o main
