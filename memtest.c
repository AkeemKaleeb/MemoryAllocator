// Shreya Shukla & Kaileb Cole
// CS 214: Systems Programming
// Professor Menendez
// Programming Assignment #1
// 02-19-2024

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


// Compile with -DREALMALLOC to use the real malloc() instead of mymalloc()
#ifndef REALMALLOC
#include "mymalloc.h"
#endif

#define MEMSIZE 4096
#define HEADERSIZE 16
#define OBJECTS 64
#define OBJSIZE (MEMSIZE / OBJECTS - HEADERSIZE)

//this test was provided in the main method of memtest but we moved it to a function so we can call other tests
int test1(){
	//printf("Objsize is %d bytes\n", OBJSIZE); //prints out the number of bytes in each object 
	char *obj[OBJECTS]; 
	int i, j, errors = 0;
	
	// fill memory with objects
	for (i = 0; i < OBJECTS; i++) {
		//printf("\n*****************************************************************************************************************************************");
		//printf ("\n\nMallocing object no. %d!\n", i);
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

//testing the coalescing of the free function
int test2(){
	char *obj[OBJECTS]; 
	int i, j, errors = 0;
	
	for (i = 0; i < OBJECTS; i++) {
		obj[i] = malloc(OBJSIZE);
	}
	//frees odd-numbered objects [1,3,5,7,9,11.....63]
	for (i = 1; i < OBJECTS; i=i+2) {
		free(obj[i]);
	}

	//frees even-numbered objects [0,2,4,6,8,10.....62]
	for (i = 0; i < OBJECTS; i=i+2) {
		free(obj[i]);
	}
	malloc(510);
	return EXIT_SUCCESS;
}



int main(int argc, char **argv)
{
	/*
	if (test1() != 0){
		printf ("Test 1 failed!");
	}
	*/
	if (test2() != 0){
		printf ("Test 2 failed!");
	}
	return EXIT_SUCCESS;
}
