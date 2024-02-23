// Shreya Shukla & Kaileb Cole
// CS 214: Systems Programming
// Professor Menendez
// Programming Assignment #1
// 02-19-2024

#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"
#include <string.h>

#define MEMLENGTH 512               // Default memory size in 8 byte chunks
static double memory[MEMLENGTH];    // Memory heap of 8 byte sections

// the Chunk datatype represents the header/metadata of each payload
typedef struct Metadata {       // 16 byte struct to store the metadata //aka 2 objects in the memory array
    char isAllocated;           // 1 bytes: 0 (free), 1(allocated)
    char padding0[5];           // 5 bytes of padding to total 16 byte struct
    short chunkDataSize;        // 2 bytes : chunkDataSize represents the size of the data plus the header
    struct Metadata *next;      // 8 bytes : points to the next metadata header
} Chunk;

//helper function to measure memory invariance
void printTotalChunkDataSize (){
    int sum = 0; //total bytes
    int i = 0;
    Chunk * start = (Chunk*) memory;
    //printf("\n-------\n");
    while (start!= NULL && ( (char *) start < ((char *) memory + sizeof(memory)))){
        sum += start->chunkDataSize; //add up the sum of all the objects in the array
        char* A = (char *) start -> next;
        char* B= (char *) start + start->chunkDataSize * 8;
        if (A!=B && A != NULL){ //checking to make sure the next chunk is allocated at the correct address
            printf("A not equal to B\n");
            printf("A: %p\n", A);
            printf("B: %p\n", B);
            abort();
        }
        //printf(" %d: size %d, total %d\n", i, start->chunkDataSize, sum);
        start = start -> next;
        i++;
    }
    if (sum != MEMLENGTH){
        printf ("Sum of heap is not equal to size of individual chunks\n");
        printf("Total heap size is: %d\n", sum);
        abort();
    }

}
//print the contents of each Chunk aka Metadata
void printChunk (Chunk* ptr) {
    printf("\n** Printing Chunk ***\n");
    printf("isAllocated?: %s\n", ptr->isAllocated ? "TRUE" : "FALSE");
    printf("chunkDataSize: %hu objects, aka %d bytes\n", ptr->chunkDataSize, (ptr->chunkDataSize * 8));

    char* memoryStart = (char *) memory;
    char* memoryEnd = (char *) memory + MEMLENGTH * 8;
    printf ("memory Start: %p", (void *) memoryStart);
    printf ("\nmemory End: %p\n", (void *) memoryEnd);

    printf("Address of Chunk: %p\n", (void *) ptr);
    printf("Address of next Chunk Header: %p\n", (void *) ptr->next);
    printf("Difference between next and current Chunks: %p\n\n", (void *)(ptr->next - ptr));
}

//prints all the location of all the Metadata objects (Chunks) in the memory array
void printAllChunks (){
    Chunk * start = (Chunk *) memory;
    while (!(start == NULL) && ( (char *) start < ((char *) memory + sizeof(memory)))){
        printChunk (start);
        start = start -> next;
    }
}
void* mymalloc(size_t size, char *file, int line) {                 // function to allocate memory in 8 byte chunks to the heap
    //printf ("Mallocing an object!\n");
    size_t alignedSize = ((size + sizeof(Chunk) + 7) & ~7) /8;      // round up to the nearest multiple of 8 for allignment including metadata
    int headerSize = sizeof(Chunk) / 8;                             // headerSize should be 2 objects, since 16 bytes = 2 doubles

    //Initialization:
    Chunk *firstChunk = (Chunk*)memory;
    if(!firstChunk->isAllocated && firstChunk->next == NULL && alignedSize != MEMLENGTH) {

        firstChunk->chunkDataSize = alignedSize;                    //assigns the first Chunk the amount of space needed for the current object                
        firstChunk->isAllocated = 1;                                //sets the first Chunk as allocated

        //Split the entire array:
        firstChunk->next = ((void *) memory + alignedSize * 8);     //assigns next Chunk
        firstChunk->next->chunkDataSize = MEMLENGTH - alignedSize;  //sets the size of the Chunk to the remaining memory space
        firstChunk->next->isAllocated = 0;                          //assigns next Chunk as free
        firstChunk->next->next = NULL;                              //points to the end of the array

        //printf("First Chunk Allocated.\n");
        //printChunk(firstChunk);
        return (void *)((char *) firstChunk + sizeof(Chunk));       //returns where the memory should be stored by adding sizeof(Chunk) bytes to the metadata address
    }
    printTotalChunkDataSize();

    Chunk *start = (Chunk*)memory;                                                 
    while (!(start == NULL) && ( (char *) start < ((char *) memory + sizeof(memory)))){ //as long as start isn't NULL and is within the range of memory               
        
        if(!start->isAllocated && start->chunkDataSize >= alignedSize) {                //checks if start has enough space to assign memory for the object we are mallocing         
            
            if(start->chunkDataSize >= (alignedSize + headerSize)) {                    //if there is extra space to create another Chunk              
                //printf("SPLITTING CHUNK!\n");
                Chunk *newChunk = (Chunk *)((char *)start + alignedSize * 8);           //create a new chunk
                
                if ((char *) newChunk < ((char *) memory + sizeof(memory))) {           //check if the newChunk is within the range of memory
                    //printf ("The newChunk is within bounds!\n");
                    newChunk->chunkDataSize = start->chunkDataSize - alignedSize;       //assign newChunk the remaining space in current chunk
                    newChunk->isAllocated = 0;                                          //set newChunk to be free
                    newChunk->next = start->next;                                       //assign newChunk -> next

                    start->next = newChunk;                                             //start should point to newChunk
                    start->chunkDataSize = alignedSize;                                 //start should have the alignedSize number of doubles
                    //printf("New Chunk Created by Splitting Current Chunk!\n");
                    //printf("\nPrinting contents of new chunk down below!\n");
                    //printChunk(newChunk);
                }
            }

            start->isAllocated = 1;                     //either way, set start to be allocated (not free)                                                   
            //printf("Allocated chunk!\n");
            //printf("\nPrinting contents of the allocated chunk down below!\n");
            //printChunk(start);
            return (void *)(start + 1);                 //return the address where the memory will be allocated                        
        }
        start = start->next;              //Moves to the next chunk (metadata) in the memory heap
    }

    printf("ERROR: Space not available (%s:%d)\n", file, line);
    return NULL;
}

// Free Memory
//needs to deal with 3 errors: calling free with an address not obtained from malloc, calling free with an address not
//at the start of the chunk, calling free a second time on the same pointer 
    
//my version of free - I translated the pseudocode provided to C
void myfree(void *ptr, char *file, int line) {
    printTotalChunkDataSize();
    // Attempting to free a NULL Pointer Error
    if(!ptr) {
        printf("ERROR: Attempting to free a NULL pointer. (%s:%d)\n", file, line);
        return;
    }

    // Get a pointer to the metadata
    Chunk *ptrChunk = (Chunk*)ptr - 1; //casts ptr to Chunk* and moves the ptr back by the size of one metadata structure
    // Print the memory addresses of ptrChunk and Chunk
    //printf("Memory address of ptrChunk: %p\n", (void*)ptrChunk);
    //printf("Memory address of ptr: %p\n", (void*)ptr);

    // Check if the pointer is out of range (before the start of the memory or after the end of memory)
    if((char*)ptrChunk< (char*)memory || (char*)ptrChunk >= (char*)(memory + MEMLENGTH * 8)) {
        printf("ERROR: Attempting to free a pointer outside the memory heap. (%s:%d)\n", file, line);
        return;
    }

    // Check if the chunk is already free
    if(!ptrChunk->isAllocated) {
        printf("ERROR: Attempting to free a pointer that is already freed. (%s:%d)\n", file, line);
        return;
    }
    ptrChunk->isAllocated = 0; //marks pointer as free
    //int headerSize = sizeof(Chunk) / 8;                      //headerSize should be 2 objects, since 16 bytes = 2 doubles

    // Attempting to free a NULL Pointer Error
    Chunk* start = (Chunk*) memory;  //start points to the beginning of the portion of the memory array we are examining
                                    //which increments as we traverse through the array

    while ((start != NULL) && (char *) start < (char *) (memory + MEMLENGTH * 8)){
        //if the ptr block is adjacent to the starting block and both blocks are free
        if (start->isAllocated == 0 && (char *) start-> next == (char *) ptrChunk && ptrChunk->isAllocated == 0){
            //merge aka coalesce adjacent blocks <START> <PTR>
            start->chunkDataSize += ptrChunk->chunkDataSize;
            start->next = ptrChunk->next;
            //ptrChunk->isAllocated = 0;
            if ((ptrChunk->next != NULL) && start->isAllocated == 0 && ptrChunk->next->isAllocated == 0){
                //merge adjacent blocks <PTR> <NEXT>
                start->chunkDataSize += ptrChunk->next->chunkDataSize;
                start->next = ptrChunk->next->next;
            }
            ptrChunk = NULL; //invalidate pointer
            return;
        }

        if ((char *) start == (char *)ptrChunk){    //if the block we're currently looking at is the pointer we wanted to free
            if ((ptrChunk->next != NULL) && ptrChunk->next->isAllocated == 0){  //coalesce
                //merge adjacent blocks <PTR> <NEXT>
                ptrChunk->chunkDataSize += ptrChunk->next->chunkDataSize;
                ptrChunk->next = ptrChunk->next->next;
            }
            //ptrChunk->isAllocated = 0; //marks pointer as free
            return;
        }
        start = start->next;
    }
    printf ("ERROR: The specified pointer was not found in heap.(%s:%d)\n", file, line);
    return;
}
