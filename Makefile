CC=gcc
CFLAGS=-g -Wall -fsanitize=address -std=c99


memtest: memtest.o mymalloc.o
	$(CC) $(CFLAGS)	$^ -o $@
