CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: week3

week3: main.o linkedlist.o
	${CC} main.o  linkedlist.o -o week3

main.o: main.c
	${CC} ${CFLAGS} main.c

linkedlist.o: linkedlist.c
	${CC} ${CFLAGS} linkedlist.c

clean:
	rm -f *.o *~
