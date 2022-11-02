CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: week1

week1: main.o linkedlist.o
	${CC} main.o  linkedlist.o -o week1

main.o: main.c
	${CC} ${CFLAGS} main.c

linkedlist.o: linkedlist.c
	${CC} ${CFLAGS} linkedlist.c

clean:
	rm -f *.o *~
