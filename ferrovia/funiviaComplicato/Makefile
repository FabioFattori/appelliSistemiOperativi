CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200112L
LIBRARIES=-lpthread
LFLAGS=

all: main.exe

main.exe: main.o
	gcc ${LFLAGS} -o main.exe main.o -lm ${LIBRARIES}
	
main.o: main.c
	gcc -c ${CFLAGS} main.c


.PHONY: clean

clean: rm -f main.o main.exe *~core
