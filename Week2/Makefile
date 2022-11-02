CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: resolver

resolver: resolver.o
	${CC} resolver.o  -o resolver

main.o: resolver.c
	${CC} ${CFLAGS} resolver.c

clean:
	rm -f *.o *~
