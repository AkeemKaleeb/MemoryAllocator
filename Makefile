CC=gcc
CFLAGS=-g -Wall -fsanitize=address -std=c99

memgrind: memgrind.o mymalloc.o
	$(CC) $(CFLAGS) $^ -o $@
	
memtest: memtest.o mymalloc.o
	$(CC) $(CFLAGS) $^ -o $@
