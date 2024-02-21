#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


// Compile with -DREALMALLOC to use the real malloc() instead of mymalloc()
#ifndef REALMALLOC
#include "mymalloc.h"
#endif

#define MEMSIZE 640
#define HEADERSIZE 16
#define OBJECTS 10
#define OBJSIZE (MEMSIZE / OBJECTS - HEADERSIZE)

int main(int argc, char **argv)
{
	printf("Objsize is %d bytes\n", OBJSIZE); //prints out the number of bytes in each object 
	char *obj[OBJECTS]; 
	int i, j, errors = 0;
	
	// fill memory with objects
	for (i = 0; i < OBJECTS; i++) {
		printf ("\n\nMallocing object no. %d!\n", i);
		obj[i] = malloc(OBJSIZE); //mallocing each object in the array
	}
	
	// fill each object with distinct bytes
	for (i = 0; i < OBJECTS; i++) {
		memset(obj[i], i, OBJSIZE); //sets obj[i] = i (with OBJSIZE bytes)
	}
	
	// check that all objects contain the correct bytes
	for (i = 0; i < OBJECTS; i++) {
		for (j = 0; j < OBJSIZE; j++) {
			if (obj[i][j] != i) {
				errors++;
				printf("Object %d byte %d incorrect: %d\n", i, j, obj[i][j]);
			}
		}
	}
	
	printf("%d incorrect bytes\n", errors);
	
	return EXIT_SUCCESS;
}
