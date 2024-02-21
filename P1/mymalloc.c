// Kaileb Cole & Shreya Shukla
// CS 214: Systems Programming
// Professor Menendez
// Programming Assignment #1
// 02-19-2024

#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"


#define MEMLENGTH 512              // Default memory size in 8 byte chunks
static double memory[MEMLENGTH];   // Memory heap of 8 byte sections


typedef struct Metadata {       // 16 byte struct to store the metadata
    char isAllocated;           // 1 bytes: 0 (free), 1(allocated)
    char padding0[5];           // 5 bytes of padding to total 16 byte struct
    short chunkDataSize;        // 2 bytes, worked on with objects, not in bytes
    struct Metadata *next;      // 8 bytes
}Chunk;


// Allocate Memory
void* mymalloc(size_t size, char *file, int line) {             // function to allocate memory in 8 byte chunks to the heap
    size_t alignedSize = ((size + sizeof(Chunk) + 7) & ~7) / 8;     // round up to the nearest multiple of 8 for allignment including metadata
    int headerSize = (int)(sizeof(Chunk) / 8);
    printf("Aligned Size: %d\n\n", alignedSize);

    Chunk *firstChunk = (Chunk*)memory;                         // set up first block for the entire memory region
    if(!firstChunk->isAllocated) {                        // initialize the memory heap if not already                    
        firstChunk->chunkDataSize = MEMLENGTH - alignedSize;  // assign data of chunk to the entire, empty heap
        firstChunk->isAllocated = 1;                            // start free
        // splitting into the next chunk
        firstChunk->next = (Chunk*)memory + firstChunk->chunkDataSize;                                // no other chunks exist, next is NULL
        firstChunk->next->isAllocated = 0;
        firstChunk->next->next = NULL;
        printf("First Chunk Initialized. Size: %d, Allocated: %d\n", firstChunk->chunkDataSize, firstChunk->isAllocated);
        return firstChunk;
    }

    while(firstChunk != NULL) {                                                           // loop through all of the chunks
        if(!firstChunk->isAllocated && firstChunk->chunkDataSize >= alignedSize) {      // check if the current chunk is free and large enough
            printf("Found suitable chunk\n");
            if(firstChunk->chunkDataSize >= alignedSize + headerSize) {                   // check if the current chunk can be split into two chunks, makes sure this has enough space for a second header plus the current header and data
                // split the chunk into allocated and free parts
                printf("Splitting chunk\n");
                Chunk *newChunk = (Chunk*)(firstChunk + alignedSize*8);              // point to a new ending chunk after the position of the data length
                newChunk->chunkDataSize = firstChunk->chunkDataSize - alignedSize;        // assign the new chunks size to the remainder of the previous size minus the current size
                newChunk->isAllocated = 0;                                                  // set the end chunk metadata to not allocated
                newChunk->next = firstChunk->next;                                        // reassign the next chunk positions
                firstChunk->next = newChunk;
                firstChunk->chunkDataSize = alignedSize - headerSize;
                printf("New Chunk Created: chunkDataSize=%d\n", newChunk->chunkDataSize);
            }

            firstChunk->isAllocated = 1;              // former end chunk can now hold data
            printf("Allocated chunk: chunkDataSize=%d\n", firstChunk->chunkDataSize);
            return (void*)(firstChunk + 1);           // returns data stored one chunk length after the start of our current chunk: equivalent to payload location
        }

        firstChunk = firstChunk->next;              //Moves to the next chunk in the memory heap
    }
    
    printf("ERROR: Space not available (%s:%d)\n", __FILE__, __LINE__);
    return NULL;
}

// test malloc function
/*void* mymalloc(size_t size, char *file, int line){
    if(size == 0) {
        printf("ERROR: Cannot allocate 0 bytes");
        return NULL;
    }

    size_t alignedSize = ((size + 7) & ~7)/8; // number of objects, not bytes
    void* resPtr = NULL;
    Chunk* start = (Chunk*)memory;

    while((char *) start < (char *)(memory + MEMLENGTH*8)) {
        int chunkSize = start->chunkDataSize;
        int isAllocated = start->isAllocated;

        if(chunkSize == 0 && !isAllocated) {
            start->chunkDataSize = alignedSize + 2;
            start->isAllocated = 1;
            resPtr = start + 2;
            //start->next->chunkDataSize = (MEMLENGTH - (start->chunkDataSize + alignedSize + 2));
            start->next->chunkDataSize = ((int)(((char*)memory + MEMLENGTH*8) - (char*)start) - alignedSize*8)/8;
            return resPtr;
        }

        if(start->isAllocated && start->chunkDataSize >= alignedSize + 2) {
            start->chunkDataSize = alignedSize + 2;
            start->isAllocated = 1;
            resPtr = start + 2;
            if(!start->next->isAllocated) {
                start->next->chunkDataSize = (chunkSize - (alignedSize + 2));
                return resPtr;
            }
            if(!start->isAllocated || chunkSize < alignedSize + 2) {
                start = start->next;
            }
        }

        printf("ERROR: not enough memory");
        return NULL;
    }
}*/

// Free Memory
//needs to deal with 3 errors: calling free with an address not obtained from malloc, calling free with an address not
//at the start of the chunk, calling free a second time on the same pointer 
    
//my version of free - I translated the pseudocode provided to C
void myfree(void *ptr, char *file, int line) {
    int headerSize = (int)(sizeof(Chunk) / 8);

    // Attempting to free a NULL Pointer Error
    if(!ptr) {
        printf("ERROR: Attempting to free a NULL pointer. (%s:%d)\n", __FILE__, __LINE__);
        return;
    }

    // Get a pointer to the metadata
    Chunk *ptrChunk = (Chunk*)ptr - 1; //casts ptr to Chunk* and moves the ptr back by the size of one metadata structure
    printf("Address provided: %p", ptr);
    printf("Chunk: %p", ptrChunk);

    // Check if the pointer is out of range (before the start of the memory or after the end of memory)
    if((char*)ptrChunk< (char*)memory || (char*)ptrChunk >= (char*)(memory + MEMLENGTH*8)) {
        printf("ERROR: Attempting to free a pointer outside the memory heap. (%s:%d)\n", __FILE__, __LINE__);
        return;
    }

    // Check if the chunk is already free
    if(!ptrChunk->isAllocated) {
        printf("ERROR: Attempting to free a pointer that is already freed. (%s:%d)\n", __FILE__, __LINE__);
        return;
    }

    // Attempting to free a NULL Pointer Error
    Chunk* start = (Chunk*) memory;  //start points to the beginning of the portion of the memory array we are examining
                                    //which increments as we traverse through the array

    while ( (char *) start < (char *)(memory + MEMLENGTH*8)){
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
