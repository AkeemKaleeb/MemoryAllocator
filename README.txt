// Shreya Shukla & Kaileb Cole
// CS 214: Systems Programming
// Professor Menendez
// Programming Assignment #1
// 02-19-2024

NETIDs: ss4515 and kjc265
For this project, we created our own implementation of the mallloc library, writing both the malloc and free programs in mymalloc.c.

We also created our own test cases: memtest.c is a program which tests the correctness of malloc, wheras memgrind.c measures the speed of malloc and free for performance testing purposes. 

Usage: To compile these programs, type "make <filename>" (where filename is either "memtest" or "memgrind) in the terminal and hit enter. Then type "./filename" and hit enter.

Sanity Testing in Memtest.c
---------------------------


Perfomance Testing in Memgrind.c
---------------------------------------------------------------------------------------------
Test1(), Test2(), and Test3() are the provided test cases in the document and are as follows:
Test1(): malloc() and immediately free() a 1-byte object, 120 times
Test2(): Use malloc() to get 120 1-byte objects, storing the pointers in an array, then use free() to deallocate the chunks.
Test3(): Create an array of 120 pointers. Repeatedly make a random choice between allocating a 1-byte object and adding the pointer to the array and deallocating a previously allocated object (if any), until you have allocated 120 times. Deallocate any remaining objects.

Finally, Test4() and Test5() are of our own creation and are as follows:
Test4(): Allocates 120 chunks of 1 byte memory to the heap and deallocates them, in reverse, every other chunk order
Test5(): Tests the error handling of the entire program , specifically the detectable errors provided in the Assignment by allocating, and freeing in imporper order followed by more allocation

---------------------------------------------------------------------------------------------
//menny has instructed to me to add some sanity tests on top of my performance tests
//try allocating only one object?? (not sure if this is what he meant)
//update README

//alllocate all of memory now it should
//allocate all of memory and then free it and then try an allocation it should work
//testing coalesce
//allocate all of memory in 4 block X0X0 XXXX and it should work
//trying a bunch
//4080 bytes
//memsize 512 -> 1024
//make sure malloc does not overlapp any other object//

sanity tests:
