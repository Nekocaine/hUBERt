CC=gcc
CFLAGS=-I. -Wall
DEPS = send.h tab.h
all : hubert restoCuisine

hubert: hubert.c hubertResto.h
	gcc $(CFLAGS) -o $@ $^

restoCuisine: restoCuisine.c semaphore.h hubertResto.h
	gcc $(CFLAGS) -o $@ $^

clean:
	rm -f *.o restoCuisine hubert
