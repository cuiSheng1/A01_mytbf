CC=g++
CFLAGS= -Wall -g -O -fPIC
INCLUDE= -I

TARGET=libmytbf.so
OBJS=mytbf.o
SRCS=mytbf.cpp
CL_PATH=/home/cl/cl/

all: $(TARGET) client server


$(TARGET):$(OBJS)
	$(CC) -shared -fPIC -o $(TARGET) $(OBJS)

$(OBJS):$(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS)


client: tcpClient.o
	$(CC) $^ -o $@

server: tcpServer.o mytbf.h
	$(CC) $^ -lmytbf -L ./ -o $@



	cp ./$(TARGET) $(CL_PATH)/lib
	cp ./client ./server $(CL_PATH)/bin


clean:
	rm -rf *.o client server $(TARGET)
