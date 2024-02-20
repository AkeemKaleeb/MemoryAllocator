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
        void* ptr = malloc(1);
        printf("obj[%d] is allocated: %d\n", i, ((char)ptr - 16));

        free(ptr);
        printf("obj[%d] is allocated: %d\n", i, ((char)ptr - 16));
    }
}

// Use malloc() to get 120 1-byte objects, storing the pointers in an array, then use free() to deallocate the chunks
void test2() {
    char obj[120];

    for (int i = 0; i < 120; i++) {
        obj[i] = malloc(1);
        printf("obj[%d] is allocated: %d\n", i,((char)obj[i] - 16));
    }

    for(int i = 0; i < 120; i++) {
        free(obj[i]);
        printf("obj[%d] is allocated: %d\n", i,((char)obj[i] - 16));
    }
}
int main(int argc, char **argv)
{
    test1();
    test2();
}