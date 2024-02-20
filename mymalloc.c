// Kaileb Cole & Shreya Shukla
// CS 214: Systems Programming
// Professor Menendez
// Programming Assignment #1
// 02-19-2024

#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"


#define MEMLENGTH 1024             // Default memory size in 8 byte chunks
static double memory[MEMLENGTH];   // Memory heap of 8 byte sections


typedef struct Metadata {       // 16 byte struct to store the metadata
    char isAllocated;           // 1 bytes: 0 (free), 1(allocated)
    char padding0[5];           // 5 bytes of padding to total 16 byte struct
    short chunkDataSize;        // 2 bytes
    struct Metadata *next;      // 8 bytes
}Chunk;


// Allocate Memory
void* mymalloc(size_t size, char *file, int line) {             // function to allocate memory in 8 byte chunks to the heap
    size_t alignedSize = ((size + sizeof(Chunk) + 7) & ~7);     // round up to the nearest multiple of 8 for allignment including metadata


    Chunk *firstChunk = (Chunk*)memory;                         // set up first block for the entire memory region
    if(firstChunk->chunkDataSize == 0) {                        // initialize the memory heap if not already                    
        firstChunk->chunkDataSize = MEMLENGTH - sizeof(Chunk);  // assign data of chunk to the entire, empty heap
        firstChunk->isAllocated = 0;                            // start free
        firstChunk->next = NULL;                                // no other chunks exist, next is NULL
        printf("First Chunk Initialized. Size: %d, Allocated: %d\n", firstChunk->chunkDataSize, firstChunk->isAllocated);
    }


    Chunk *currentChunk = (Chunk*)memory;                                                   // start search at the beginning of the memory heap
    while(currentChunk != NULL) {                                                           // loop through all of the chunks
        printf("Checking chunk: isAllocated=%d, chunkDataSize=%d, alignedSize=%zu\n", currentChunk->isAllocated, currentChunk->chunkDataSize, alignedSize);
        if(!currentChunk->isAllocated && currentChunk->chunkDataSize >= alignedSize) {      // check if the current chunk is free and large enough
            printf("Found suitable chunk\n");
            if(currentChunk->chunkDataSize >= alignedSize + sizeof(Chunk)) {                // check if the current chunk can be split into two chunks
                // split the chunk into allocated and free parts
                printf("Splitting chunk\n");
                Chunk *newChunk = (Chunk*)((char*)currentChunk + alignedSize);              // point to a new ending chunk after the position of the data length
                newChunk->chunkDataSize = currentChunk->chunkDataSize - alignedSize;        // assign the new chunks size to the remainder of the previous size minus the current size
                newChunk->isAllocated = 0;                                                  // set the end chunk metadata to not allocated
                newChunk->next = currentChunk->next;                                        // reassign the next chunk positions
                currentChunk->next = newChunk;
                printf("New Chunk Created: chunkDataSize=%d\n", newChunk->chunkDataSize);
            }


            currentChunk->isAllocated = 1;              // former end chunk can now hold data
            currentChunk->chunkDataSize = alignedSize;  // set the size of the allocated chunk
            printf("Allocated chunk: chunkDataSize=%d\n", currentChunk->chunkDataSize);
            return (void*)(currentChunk + 1);           // returns data stored one chunk length after the start of our current chunk: equivalent to payload location
        }


        currentChunk = currentChunk->next;              //Moves to the next chunk in the memory heap
    }


    printf("ERROR: Space not available (%s:%d)\n", __FILE__, __LINE__);
    return NULL;
}


// Free Memory
//needs to deal with 3 errors: calling free with an address not obtained from malloc, calling free with an address not
//at the start of the chunk, calling free a second time on the same pointer 

/* The following pseudocode was provided to us in Recitation: 

    void myfree(void ptr) {
    charstart = memoryStart;

    while (start < memoryEnd) {
        if (isAdjacentAndFree(start, ptr)) {
            mergeBlocks(start, ptr);
            if (isFree(nextBlock(ptr))) {
                mergeBlocks(start, nextBlock(ptr));
            }
            invalidatePointer(ptr);
            return;
        }

        if (start == getMetadataStart(ptr)) {
            if (isFree(nextBlock(ptr))) {
                mergeBlocks(ptr, nextBlock(ptr));
            }
            markAsFree(ptr);
            invalidatePointer(ptr);
            return;
        }

        start = nextBlock(start);
    }

    exitWithError("Pointer not in heap");
}
*/
    
//my version of free - I translated the pseudocode provided to C
void myfree(void *ptr, char *file, int line) {

    // Attempting to free a NULL Pointer Error
    if(!ptr) {
        printf("ERROR: Attempting to free a NULL pointer. (%s:%d)\n", __FILE__, __LINE__);
        return;
    }

    // Get a pointer to the metadata
    Chunk *ptrChunk = (Chunk*)ptr - 1; //casts ptr to Chunk* and moves the ptr back by the size of one metadata structure

    // Check if the pointer is out of range (before the start of the memory or after the end of memory)
    if((char*)ptrChunk< (char*)memory || (char*)ptrChunk >= (char*)memory + MEMLENGTH) {
        printf("ERROR: Attempting to free a pointer outside the memory heap. (%s:%d)\n", __FILE__, __LINE__);
        return;
    }

    // Check if the chunk is already free
    if(!ptrChunk>isAllocated) {
        printf("ERROR: Attempting to free a pointer that is already freed. (%s:%d)\n", __FILE__, __LINE__);
        return;
    }

    // Attempting to free a NULL Pointer Error
    Chunk* start = (Chunk*) memory;  //start points to the beginning of the portion of the memory array we are examining
                                    //which increments as we traverse through the array

    while ( (char *) start < (char *) (memory + MEMLENGTH)){
        //if the ptr block is adjacent to the starting block and both blocks are free
        if (start->isAllocated == 0 && (char *) start-> next == (char *) ptrChunk && ptrChunk->isAllocated == 0){
            //merge adjacent blocks <START> <PTR>
            start->chunkDataSize += ptrChunk->chunkDataSize + sizeof(Chunk);
            start->next = ptrChunk->next;
            ptrChunk->isAllocated = 0;
            if (start->isAllocated == 0 && ptrChunk->next->isAllocated == 0){
                //merge adjacent blocks <PTR> <NEXT>
                start->chunkDataSize += ptrChunk->next->chunkDataSize + sizeof(Chunk);
                start->next = ptrChunk->next->next;
            }
            ptrChunk = NULL; //invalidate pointer
            return;
        }

        if ((char *) start == (char *)ptrChunk){
            if (ptrChunk->next->isAllocated == 0){ 
                //merge adjacent blocks <PTR> <NEXT>
                ptrChunk->chunkDataSize += ptrChunk->next->chunkDataSize + sizeof(Chunk);
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
/* Kaileb's version of free
void myfree(void *ptr, char *file, int line) {
    // Attempting to free a NULL Pointer Error
    if(!ptr) {
        printf("ERROR: Attempting to free a NULL pointer. (%s:%d)\n", __FILE__, __LINE__);
        return;
    }


    // Get a pointer to the metadata
    Chunk *chunkToFree = (Chunk*)ptr - 1; //casts ptr to Chunk* and moves the ptr back by the size of one Chunk structure

    // Check if the pointer is at the start of a chunk
    if((char*)chunkToFree < (char*)memory || (char*)chunkToFree >= (char*)memory + MEMLENGTH) {
        printf("ERROR: Attempting to free a pointer outside the memory heap. (%s:%d)\n", __FILE__, __LINE__);
        return;
    }


    // Check if the chunk is already free
    if(!chunkToFree->isAllocated) {
        printf("ERROR: Attempting to free a pointer that is already freed. (%s:%d)\n", __FILE__, __LINE__);
        return;
    }


    // Mark the chunk as free;
    chunkToFree->isAllocated = 0;
   
    // Coalesce free blocks
    Chunk *currentChunk = (Chunk*)memory;
    while(currentChunk != NULL && currentChunk->next != NULL) { //iterates through the chunks untill you reach the end of the memory heap
        if(!currentChunk->isAllocated && !currentChunk->next->isAllocated) {
            // Merge current chunk with the next chunk
            currentChunk->chunkDataSize += currentChunk->next->chunkDataSize + sizeof(Chunk);
            currentChunk->next = currentChunk->next->next;
        }


        currentChunk = currentChunk->next;
    }
}
*/
