Johnny Chen

The data from the command line is read into the program using <getop.h> library. For step 1, the program first initialize 2 array using charFreq struct: one that stores the ASCII value from 0-255 and another one stores their frequencies. Then the file data from the command line is read by the FILE structure from the <stdio.h> library. For each character read, the corresponding freq field in the charFreq array is incremented-this will create a frequency table of ASCII value <-> frequency. To print out the frequencies for debug, the program search through the charFreq array and print them out (excluding zero frequency characters)

In 2a, the program creates a forest of tree nodes by calling createNode() for each frequency in charFreq with their corresponding character inside their nodes (exclude 0 count frequencies). The createNode() takes the frequency, setting its left and right child to NULL and return its frequency or character if asked.

In 2b, the program enqueue() these nodes into a min heap priority queue so they can be sorted using qsort() from <stdlib.h>. This will ensure that the two nodes with the lowest frequency is at the beginning of the queue. To build the Huffman tree, the priority queue is put into buildHuffman(). This function works by dequeue() the two lowest frequency from the queue and create a node with those nodes as children; resort and repeat until there's one node left in the queue and that is assigned to a variable *root. For debug, the program prints out the *root which represents total frequency.

In step 3, traverseHuffmanTree() is called by using the *root and 0 (depth of the tree) as parameter. The function traverses the tree from the top (depth 0) to each node that has NULL for its left & right children and collect their code (represented by the edges) into an array (binary[]) for format before sending it into codes[] array. Debug is to print out each character with its code by going through the codes[] array. 

To get the lossless compressed file in step 4, I used the FILE structure again to create and open up a new file then read the input file again. Character by character, it looks up its corresponding Huffman code in the codes[] from step 3 and write the corresponding code into a buffer. Whenever the buffer contains a byte, it writes that byte to the output file. Using bit shift << and bitwise | operator will ensure this behavior. Then any leftover bits are written after the input file is finish reading. 

References:

I want to thank GeeksforGeeks and the book "Algorithms in C" by Robert Sedgewick for guiding me throughout this project. 

Robert explains in further details about the steps in building the Huffman Tree. He suggested to use priority queue and use that to create min heap (he called it downheap). He provided examples of these data structure implementation. 

GeeksforGeeks helped me implement a min heap structure. 
 