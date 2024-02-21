// Kaileb Cole & Shreya Shukla
// CS 214: Systems Programming
// Professor Menendez
// Programming Assignment #1
// 02-19-2024

#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"

#define MEMLENGTH 80            // Default memory size in 8 byte chunks
static double memory[MEMLENGTH];   // Memory heap of 8 byte sections
int spaceRemaining = MEMLENGTH; //this is a global variable and we will REMOVE it later


typedef struct Metadata {       // 16 byte struct to store the metadata //aka 2 objects in the memory array
    char isAllocated;           // 1 bytes: 0 (free), 1(allocated)
    char padding0[5];           // 5 bytes of padding to total 16 byte struct
    short chunkDataSize;        // 2 bytes : chunkDataSize represents the size of the data plus the header
    struct Metadata *next;      // 8 bytes : points to the next metadata header
} Chunk;

//print the contents of each Chunk Metadata
void printChunk (Chunk* ptr) {
    printf("\n** Printing Chunk ***\n");
    printf("isAllocated?: %c\n", ptr->isAllocated);
    printf("chunkDataSize: %hu objects, aka %d bytes\n", ptr->chunkDataSize, (ptr->chunkDataSize * 8));

    char* memoryStart = (char *) memory;
    char* memoryEnd = (char *) memory + MEMLENGTH * 8;
    printf ("memory Start: %p", (void *) memoryStart);
    printf ("\nmemory End: %p\n", (void *) memoryEnd);

    printf("Address of Chunk: %p\n", (void *) ptr);
    printf("Address of next Chunk Header: %p\n\n", (void *) ptr->next);
}
void* mymalloc(size_t size, char *file, int line) {                 // function to allocate memory in 8 byte chunks to the heap

    printf ("Mallocing an object!\n");
    printf ("Space for %d objects in the array!\n", spaceRemaining);
    size_t alignedSize = ((size + sizeof(Chunk) + 7) & ~7) /8;     // round up to the nearest multiple of 8 for allignment including metadata
    int headerSize = sizeof(Chunk) / 8;                             //headerSize should be 2 objects, since 16 bytes = 2 doubles
    
    char* memoryStart = (char *) memory;
    char* memoryEnd = (char *) memory + MEMLENGTH * 8;
    //printf ("memory Start: %p", (void *) memoryStart);
    //printf ("\nmemory End: %p\n", (void *) memoryEnd);

    //Initialization:
    Chunk *firstChunk = (Chunk*)memory;

    if(!firstChunk->isAllocated && firstChunk->next == NULL) {

        firstChunk->chunkDataSize = alignedSize;                
        firstChunk->isAllocated = 1;               

        //Split the entire array:
        firstChunk->next = (Chunk*) (memory + alignedSize * 8); //assigns next Chunk
        firstChunk->next->chunkDataSize = MEMLENGTH - alignedSize;
        firstChunk->next->isAllocated = 0;
        firstChunk->next->next = NULL;

        printf("First Chunk Allocated.\n");
        printChunk(firstChunk);
        spaceRemaining -= firstChunk->chunkDataSize;
        printf("Space Remaining in the Array: %d Objects", spaceRemaining);
        return (void *)(firstChunk + 1);
    }

    Chunk *start = (Chunk*)memory;                                                 
    while ((!(start == NULL)) && ( (char *) start < (char *) (memory + MEMLENGTH * 8))){                    
        
        if(!start->isAllocated && start->chunkDataSize >= alignedSize) {            
            
            if(start->chunkDataSize >= (alignedSize + headerSize)) {//if there is extra space to create another Chunk              
                printf("Splitting chunk!\n");
                Chunk *newChunk = (Chunk*)((char*)start + alignedSize * 8);         
                
                if (!((char *)newChunk >= memoryEnd)) {
                //printf("Distance from memory end: %lx bytes", ((char *) memory + MEMLENGTH * 8 - (char *) newChunk));
                newChunk->chunkDataSize = start->chunkDataSize - alignedSize;       
                newChunk->isAllocated = 0;                                          
                newChunk->next = start->next;                                       
                start->next = newChunk;
                start->chunkDataSize = alignedSize - headerSize;
                printf("New Chunk Created by Splitting Current Chunk!\n");
                }
            }

            start->isAllocated = 1;                                                   
            printf("Allocated chunk!\n");
            //printf("\nAddress of Current Chunk: %p \n\n", (void *) start);
            printChunk(start);
            spaceRemaining -= start->chunkDataSize;
            printf("Space Remaining in the Array: %d Objects", spaceRemaining);
            return (void *)(start + 1);                                               
        }
        start = start->next;              //Moves to the next chunk in the memory heap
    }

    printf("ERROR: Space not available (%s:%d)\n", __FILE__, __LINE__);
    return NULL;
}

// Free Memory
//needs to deal with 3 errors: calling free with an address not obtained from malloc, calling free with an address not
//at the start of the chunk, calling free a second time on the same pointer 
    
//my version of free - I translated the pseudocode provided to C
void myfree(void *ptr, char *file, int line) {

    // Attempting to free a NULL Pointer Error
    if(!ptr) {
        printf("ERROR: Attempting to free a NULL pointer. (%s:%d)\n", __FILE__, __LINE__);
        return;
    }

    // Get a pointer to the metadata
    Chunk *ptrChunk = (Chunk*)ptr - 1; //casts ptr to Chunk* and moves the ptr back by the size of one metadata structure

    // Print the memory addresses of ptrChunk and Chunk
    printf("Memory address of ptrChunk: %p\n", (void*)ptrChunk);
    printf("Memory address of ptr: %p\n", (void*)ptr);

    // Check if the pointer is out of range (before the start of the memory or after the end of memory)
    if((char*)ptrChunk< (char*)memory || (char*)ptrChunk >= (char*)(memory + MEMLENGTH * 8)) {
        printf("ERROR: Attempting to free a pointer outside the memory heap. (%s:%d)\n", __FILE__, __LINE__);
        return;
    }

    // Check if the chunk is already free
    if(!ptrChunk->isAllocated) {
        printf("ERROR: Attempting to free a pointer that is already freed. (%s:%d)\n", __FILE__, __LINE__);
        return;
    }

    int headerSize = sizeof(Chunk) / 8;                      //headerSize should be 2 objects, since 16 bytes = 2 doubles

    // Attempting to free a NULL Pointer Error
    Chunk* start = (Chunk*) memory;  //start points to the beginning of the portion of the memory array we are examining
                                    //which increments as we traverse through the array

    while ( (char *) start < (char *) (memory + MEMLENGTH * 8)){
        //if the ptr block is adjacent to the starting block and both blocks are free
        if (start->isAllocated == 0 && (char *) start-> next == (char *) ptrChunk && ptrChunk->isAllocated == 0){
            //merge adjacent blocks <START> <PTR>
            start->chunkDataSize += ptrChunk->chunkDataSize + headerSize;
            start->next = ptrChunk->next;
            ptrChunk->isAllocated = 0;
            if (start->isAllocated == 0 && ptrChunk->next->isAllocated == 0){
                //merge adjacent blocks <PTR> <NEXT>
                start->chunkDataSize += ptrChunk->next->chunkDataSize + headerSize;
                start->next = ptrChunk->next->next;
            }
            ptrChunk = NULL; //invalidate pointer
            return;
        }

        if ((char *) start == (char *)ptrChunk){
            if (ptrChunk->next->isAllocated == 0){ 
                //merge adjacent blocks <PTR> <NEXT>
                ptrChunk->chunkDataSize += ptrChunk->next->chunkDataSize + headerSize;
                ptrChunk->next = ptrChunk->next->next;
            }
            ptrChunk->isAllocated = 0; //marks pointer as free
            return;
        }
        start = start->next;
    }
    printf ("ERROR: The specified pointer was not found in heap.(%s:%d)\n", __FILE__, __LINE__);
    return;
}
