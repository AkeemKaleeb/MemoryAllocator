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

	//free all the objects
	for (i = 0; i < OBJECTS; i++){
		free(obj[i]);
	}
	
	return EXIT_SUCCESS;
}

//checks if we can allocate memory, free it, allocate it again, then free it again
int test2(){
	char *obj[OBJECTS]; 
	int i;
	
	// fill memory with objects
	for (i = 0; i < OBJECTS; i++) {
		obj[i] = malloc(OBJSIZE); //mallocing each object in the array
	}
	
	// free each object
	for (i = 0; i < OBJECTS; i++) {
		free(obj[i]);
	}

	// fill memory with the same number of objects again
	for (i = 0; i < OBJECTS; i++) {
		obj[i] = malloc(OBJSIZE); //mallocing each object in the array
	}
	// free each object
	for (i = 0; i < OBJECTS; i++) {
		free(obj[i]);
	}
	return EXIT_SUCCESS;
}

//testing the coalescing of the free function
int test3(){
	char *obj[OBJECTS]; 
	int i;
	
	for (i = 0; i < OBJECTS; i++) {
		obj[i] = malloc(OBJSIZE);
	}

	//frees even-numbered objects [0,2,4,6,8,10.....62]
	for (i = 0; i < OBJECTS; i=i+2) {
		free(obj[i]);
	}
	//frees odd-numbered objects [1,3,5,7,9,11.....63]
	for (i = 1; i < OBJECTS; i=i+2) {
		free(obj[i]);
	}
	//printAllChunks();
	char * ptr = malloc(MEMSIZE - HEADERSIZE);
	free (ptr);
	return EXIT_SUCCESS;
}

//testing to see if we get a error message when trying to allocate more memory than is available
//this should print an error message
int test4(){
	char * ptr = malloc(MEMSIZE - HEADERSIZE);	//allocate all of memomory to one object;
	malloc (1); //trying to allocate more memory then is available
	free (ptr);
	return EXIT_SUCCESS;
}

//tests for error-handling messages
int test5(){
    int x;
    int *p = malloc(sizeof(int)*2);
    int *q = p;

	//except for free(p), all of these free statements should result in error messages
    free(&x); 	//this should not work since x was not allocated with malloc
    free(p + 1); //this also should not work because p + 1 points to memory that was not allocated with malloc and thus should be free
    free(p); 	//this should work since p was allocated with malloc.
    free(q);	//this should not work since q points to the same memory location as p, and p was already freed.	 
	return EXIT_SUCCESS;
}

//tests coalesce function edge case: freeing a block of memory at the beginning of the heap should still coalesce
int test6(){
	char * ptr = malloc(OBJSIZE);
	free (ptr);
	//printAllChunks();
	char * ptr2 = malloc (MEMSIZE - HEADERSIZE);
	free(ptr2);
	return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
	if (test1() != 0){
		printf ("Test 1 failed!");
	}
	if (test2() != 0){
		printf ("Test 2 failed!");
	}
	if (test3() != 0){
		printf ("Test 3 failed!");
	}
	if (test4() != 0){
		printf ("Test 4 failed!");
	}
	if (test5() != 0){
		printf ("Test 5 failed!");
	}
	if (test6() != 0){
		printf ("Test 6 failed!");
	}
	return EXIT_SUCCESS;
}
