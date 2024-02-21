// Kaileb Cole & Shreya Shukla
// CS 214: Systems Programming
// Professor Menendez
// Programming Assignment #1
// 02-19-2024

NETIDs: ss4515 and kjc265
For this project, we created our own implementation of the mallloc library, writing both malloc and free.
To run this program, type "make" in the terminal and hit enter. 

Error detection is handled with a printf statement to the console that includes the file and line location of where the error occured.

Test1(), Test2(), and Test3() are the provided test cases in the document and are as follows:
Test1(): malloc() and immediately free() a 1-byte object, 120 times
Test2(): Use malloc() to get 120 1-byte objects, storing the pointers in an array, then use free() to deallocate the chunks.
Test3(): Create an array of 120 pointers. Repeatedly make a random choice between allocating a 1-byte object and adding the pointer to the array and deallocating a previously allocated object (if any), until you have allocated 120 times. Deallocate any remaining objects.

Finally, Test4() and Test5() are of our own creation and are as follows:
Test4(): Allocates 120 chunks of 1 byte memory to the heap and deallocates them, in reverse, every other chunk order
Test5(): Tests the error handling of the entire program , specifically the detectable errors provided in the Assignment by allocating, and freeing in imporper order followed by more allocation
