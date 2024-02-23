// Shreya Shukla & Kaileb Cole
// CS 214: Systems Programming
// Professor Menendez
// Programming Assignment #1
// 02-19-2024

NETIDs: ss4515 and kjc265
For this project, we created our own implementation of the malloc library, writing both the malloc and free functions in mymalloc.c.

We also created our own test cases: memtest.c is a program which tests the correctness of malloc, wheras memgrind.c measures the speed of malloc and free for performance testing purposes. 

Usage: To compile these programs, type "make <filename>" (where filename is either "memtest" or "memgrind") in the terminal and hit enter. Then type "./filename" and hit enter.

Sanity Testing in Memtest.c
---------------------------
Test1() was provided by Professor Menendez and was originally located in the main method of memtest.c. Test1() fills the ith object in the array with i distinct bytes 
(for example object[2] payload would be 22222222... and object[3] payload would be 333333.... and so on). 
This test ensures that none of the objects overlap each other and prints an error message if they do overlap.

Test2() tests if we can allocate memory for N distinct objects, free the objects, allocate N objects again, and then free them again.

Test3() tests the coalesce function of our free method. It frees all the even-numbered objects, then all odd-numbered objects, and expects all to 
coalesce into one big free chunk of memory. Test(2) then allocates all of the memory by doing malloc(MEMSIZE - HEADERSIZE) (which in this case is 4080 bytes) and frees it.

Test4() tests what happens if we try to allocate more memory than is available in the array. Test 4 allocates all of the memory by doing malloc(MEMSIZE - HEADERSIZE),
then tries to allocate one more byte (which exceeds the amount of memory available).
If it works correctly, Test4() should print an error message: "ERROR: Space not available".

Test5() tests what should happen if we try to free memory that was not allocated with malloc, or if we try to free the same memory twice.
Test5() should produce multiple error messages.

Test6() tests the edge case where we try to allocate memory at the beginning of the heap, free it, and then allocate the entire memory in the array (by doing malloc (MEMSIZE - HEADERSIZE)).
This is to make sure that free coalesces the beginning block with the rest of the memory (which has not been allocated and is thus free). 

Performance Testing in Memgrind.c
---------------------------------------------------------------------------------------------
Test1(), Test2(), and Test3() are the provided test cases in the document and are as follows:
Test1(): malloc() and immediately free() a 1-byte object, 120 times.
Test2(): Use malloc() to get 120 1-byte objects, storing the pointers in an array, then use free() to deallocate the chunks.
Test3(): Create an array of 120 pointers. Repeatedly make a random choice between allocating a 1-byte object and adding the pointer to the array and deallocating a previously allocated object (if any), until you have allocated 120 times. Deallocate any remaining objects.

Finally, Test4() and Test5() are of our own creation and are as follows:
Test4(): Allocates 120 chunks of 1-byte memory to the heap and deallocates them, in reverse, every other chunk order.
Test5(): Tests the error handling of the entire program, specifically the detectable errors provided in the Assignment by allocating, and freeing in improper order followed by more allocation.

Memgrind.c will run each of these tests 50 times and then will print out the average runtime of these tests in microseconds. This tells us if our malloc and free functions are time-efficient or not. 
---------------------------------------------------------------------------------------------

