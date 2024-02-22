#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>


// Compile with -DREALMALLOC to use the real malloc() instead of mymalloc()
#ifndef REALMALLOC
#include "mymalloc.h"
#endif


// malloc() and immediately free() a 1-byte object, 120 times
void test1() {
    for(int i = 0; i < 120; i++) {
        void *ptr = malloc(1);
        //printf("obj[%d] is allocated: %d\n", i, *((char*)ptr - 16));

        free(ptr);
        //printf("obj[%d] is freed: %d\n", i, *((char*)ptr - 16));
    }
}

// Use malloc() to get 120 1-byte objects, storing the pointers in an array, then use free() to deallocate the chunks
void test2() {
    char *obj[120];

    for (int i = 0; i < 120; i++) {
		obj[i] = malloc(1);
        //printf("obj[%d] is allocated: %d\n", i, *((char*)obj[i] - 16));
	}

    for(int i = 0; i < 120; i++) {
		free(obj[i]);
        //printf("obj[%d] is freed: %d\n", i, *((char*)obj[i] - 16));
	}
}

// helper function for test 3, generates a random number between max and min values
int randInRange(int min, int max) {
    return min + (int) (rand() / (double) (RAND_MAX + 1) * (max - min + 1));
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

    //free(&x);
    //free(p + 1);
    //free(p);
    //free(q);

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

int main(int argc, char **argv) {
    int counter = 0;
    struct timeval start_time, end_time;
    float times[5] = {0}; // Initialize to 0
    
    gettimeofday(&start_time, NULL);
    while (counter < 5) {
        test1();
        counter++;
    }
    gettimeofday(&end_time, NULL);
    times[0] = (float)((end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_usec - start_time.tv_usec)) / 5;
    printf("Time: %f microseconds\n", times[0]);
    counter = 0;
    
    gettimeofday(&start_time, NULL);
    while (counter < 5) {
        test2();
        counter++;
    }
    gettimeofday(&end_time, NULL);
    times[1] = (float)((end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_usec - start_time.tv_usec)) / 5;
    printf("Time: %f microseconds\n", times[1]);
    counter = 0;
    
    gettimeofday(&start_time, NULL);
    while (counter < 5) {
        test3();
        counter++;
    }
    
    gettimeofday(&end_time, NULL);
    times[2] = (float)((end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_usec - start_time.tv_usec)) / 5;
    printf("Time: %f microseconds\n", times[2]);
    counter = 0;
    
    gettimeofday(&start_time, NULL);
    while (counter < 5) {
        test4();
        counter++;
    }
    gettimeofday(&end_time, NULL);
    times[3] = (float)((end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_usec - start_time.tv_usec)) / 5;
    printf("Time: %f microseconds\n", times[3]);
    counter = 0;
    
    gettimeofday(&start_time, NULL);
    while (counter < 5) {
        test5();
        counter++;
    }
    gettimeofday(&end_time, NULL);

    times[4] = (float)((end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_usec - start_time.tv_usec)) / 5;
    printf("Time: %f microseconds\n", times[4]);
    counter = 0;

    for (int i = 0; i < 5; i++) {
        printf("Average time for test%d(): %f microseconds\n", i + 1, times[i]);
    }

    return 0;
}
