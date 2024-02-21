#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


// Compile with -DREALMALLOC to use the real malloc() instead of mymalloc()
#ifndef REALMALLOC
#include "mymalloc.h"
#endif


// malloc() and immediately free() a 1-byte object, 120 times
void test1() {
    for(int i = 0; i < 120; i++) {
        void *ptr = malloc(1);
        printf("obj[%d] is allocated: %d\n", i, *((char*)ptr - 16));

        free(ptr);
        printf("obj[%d] is allocated: %d\n", i, *((char*)ptr - 16));
    }
}

// Use malloc() to get 120 1-byte objects, storing the pointers in an array, then use free() to deallocate the chunks
void test2() {
    char *obj[120];

    for (int i = 0; i < 120; i++) {
		obj[i] = malloc(1);
        printf("obj[%d] is allocated: %d\n", i, *((char*)obj[i] - 16));
	}

    for(int i = 0; i < 120; i++) {
		free(obj[i]);
        printf("obj[%d] is allocated: %d\n", i, *((char*)obj[i] - 16));
	}
}

// Create an array of 120 pointers. Repeatedly make a random choice between allocating a 1-byte
// object and adding the pointer to the array and deallocating a previously allocated object (if
// any), until you have allocated 120 times. Deallocate any remaining objects.
void test3() {
    int counter = 0;
    char* obj[120];
    while(counter < 120) {
        int random = randInRange(0, 1);
        if(random < 1) {        // less than 1 -> allocate
            obj[counter] = malloc(1);
            //printf("obj[%d] is allocated: %d\n", counter, *((char*)obj[counter] - 16));
            counter++;
        }
        else if(random >= 1) {  // greater than 1 -> free previous object
            for(int i = counter; i >= 0; i--){
                if(*((char*)obj[i] - 16) == 1) {
                    free(obj[i]);
                }
            }
        }
    }

    for(int i = 0; i < 120; i++){
        if(*((char*)obj[i] - 16) == 1) {
            free(obj[i]);
        }
    }
}

// helper function for test 3, generates a random number between max and min values
int randInRange(int min, int max) {
    return min+ (int) (rand() / (double) (RAND_MAX + 1) * (max - min + 1));
}

// Allocates 120 chunks and frees them in reverse, every other order
void test4() {
    char *obj[120];
    for (int i = 0; i < 120; i++) {
		obj[i] = malloc(1);
        //printf("obj[%d] is allocated: %d\n", i, *((char*)obj[i] - 16));
	}

    for(int i = 119; i >= 0; i -= 2) {
		free(obj[i]);
        //printf("obj[%d] is allocated: %d\n", i, *((char*)obj[i] - 16));
	}
    for(int i = 118; i >= 0; i -= 2) {
        free(obj[i]);
        //printf("obj[%d] is allocated: %d\n", i, *((char*)obj[i] - 16));
    }
}

void test5() {
    int x;
    char *obj[120];
    int *p = malloc(sizeof(int)*2);
    int *q = p;

    free(&x);
    free(p + 1);
    free(p);
    free(q);

    for (int i = 0; i < 64; i++) {
		obj[i] = malloc(1);
        //printf("obj[%d] is allocated: %d\n", i, *((char*)obj[i] - 16));
	}
    for(int i = 0; i < 64; i++) {
        if(*((char*)obj[i] - 16)) {
		    free(obj[i]);
            //printf("obj[%d] is allocated: %d\n", i, *((char*)obj[i] - 16));
	    }
    }
}

int main(int argc, char **argv)
{
    int counter = 0;
    int time = gettimeofday();
    float times[] = {5};

    while(counter < 50) {
	    test1();
    }
    times[0] = (gettimeofday() - time)/2;
    counter = 0;

    /*while(counter < 50) {
	    test2();
    }
    times[1] = (gettimeofday() - time)/2;
    counter = 0;

    while(counter < 50) {
	    test3();
    }
    times[2] = (gettimeofday() - time)/2;
    counter = 0;

    while(counter < 50) {
	    test4();
    }
    times[3] = (gettimeofday() - time)/2;
    counter = 0;

    while(counter < 50) {
	    test5();
    }
    times[4] = (gettimeofday() - time)/2;
    counter = 0;
*/
    for(int i = 0; i < 5; i++) {
        printf("Average time for test%d(): %f\n", i, times[i]);
    }
}