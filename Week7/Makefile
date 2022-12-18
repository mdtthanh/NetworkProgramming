CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: clean server client


client: stack.o client.o
	${CC} -pthread stack.o client.o -o client

server: stack.o server.o
	${CC} -pthread stack.o server.o -o server

server.o: server.c
	${CC} ${CFLAGS} -pthread server.c
	
client.o : client.c
	${CC} ${CFLAGS} -pthread client.c

stack.o : stack.c
	${CC} ${CFLAGS} stack.c

clean:
	rm -rf *.o *.~ | clear