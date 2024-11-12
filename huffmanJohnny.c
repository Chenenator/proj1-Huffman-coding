// Johnny Chen

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Define tree node
typedef struct treeNode
{
    int value;
    unsigned char character;
    struct treeNode *leftChild;
    struct treeNode *rightChild;
} treeNode;

// Function to create a node
treeNode *createNode(int value, unsigned char character)
{
    treeNode *result = malloc(sizeof(treeNode));
    if (result != NULL)
    {
        result->leftChild = NULL;
        result->rightChild = NULL;
        result->value = value;
        result->character = character;
    }
    return result;
}

// Function to insert a node into the tree
treeNode *insertNode(treeNode *root, int value, unsigned char character)
{
    if (root == NULL)
    {
        return createNode(value, character);
    }
    if (value < root->value)
    {
        root->leftChild = insertNode(root->leftChild, value, character);
    }
    else if (value > root->value)
    {
        root->rightChild = insertNode(root->rightChild, value, character);
    }
    return root;
}

// Function to compare nodes to sort by frequency
int compareNodes(const void *a, const void *b)
{
    return (*(treeNode **)a)->value - (*(treeNode **)b)->value;
}

// Struct to store characters and their frequencies
typedef struct charFreq
{
    unsigned char ch;
    int freq;
} charFreq;

// PriorityQueue struct
typedef struct
{
    treeNode *items[256];
    int size;
} priorityQueue;

/*Functions to create a minheap priority queue */
void downHeap(priorityQueue *pq, int index)
{
    int smallest = index;
    int left = 2 * index + 1;  // left child node
    int right = 2 * index + 2; // right child node
    if (left < pq->size && pq->items[left]->value < pq->items[smallest]->value)
    {
        smallest = left;
    }
    if (right < pq->size && pq->items[right]->value < pq->items[smallest]->value)
    {
        smallest = right;
    }
    if (smallest != index)
    {
        treeNode *temp = pq->items[index];
        pq->items[index] = pq->items[smallest];
        pq->items[smallest] = temp;
        downHeap(pq, smallest);
    }
}
void enqueue(priorityQueue *pq, treeNode *node)
{
    pq->items[pq->size++] = node;
    downHeap(pq, pq->size - 1);
}

treeNode *dequeue(priorityQueue *pq)
{
    treeNode *item = pq->items[0];
    pq->items[0] = pq->items[--pq->size];
    downHeap(pq, 0);
    return item;
}
/* End */

// Function to build the huffman tree
treeNode *buildHuffman(priorityQueue *pq)
{
    while (pq->size > 1)
    {
        treeNode *left = dequeue(pq);
        treeNode *right = dequeue(pq);

        treeNode *newNode = createNode(left->value + right->value, '\0');
        newNode->leftChild = left;
        newNode->rightChild = right;
        enqueue(pq, newNode);
    }
    return dequeue(pq);
}
// Function for traversing the huffman tree
void traverseHuffmanTree(treeNode *root, int depth)
{
    if (root == NULL)
    {
        return;
    }
    // Recursive travel for the left and right children
    traverseHuffmanTree(root->leftChild, depth + 1);
    traverseHuffmanTree(root->rightChild, depth + 1);
}

int combinedFrequency(treeNode *root)
{
    if (root == NULL)
    {
        return 0;
    }
    return root->value;
}

// Function to collect the huffman codes
void collectCodes(treeNode *root, char *codes[], char binary[], int top)
{
    if (root->leftChild)
    {
        binary[top] = '0'; // Assign '0' for left child (represent the edges between the nodes on the left branch)
        collectCodes(root->leftChild, codes, binary, top + 1);
    }
    if (root->rightChild)
    {
        binary[top] = '1'; // Assign '1' for right child (the edges between the nodes on the right branch)
        collectCodes(root->rightChild, codes, binary, top + 1);
    }
    // If it's a leaf node, store the bit sequence
    if (root->leftChild == NULL && root->rightChild == NULL)
    {
        binary[top] = '\0';                      // Null-terminate the string
        codes[root->character] = strdup(binary); // Store the binary seqeunce for this character
    }
}

void debugFrequencies(int debug, const charFreq *charFreq)
{
    if (debug)
    {
        for (int i = 0; i < 256; i++)
        {
            if (charFreq[i].freq != 0)
            {
                switch (i)
                {
                case '\n':
                    printf("'LF' frequency: %i,\n", charFreq[i].freq);
                    break;
                case '\t':
                    printf("'\\t' frequency: %i,\n", charFreq[i].freq);
                    break;
                default:
                    printf("'%c' frequency: %i,\n", i, charFreq[i].freq);
                }
            }
        }
    }
}

void debugPrintCodes(char *codes[256], int debug)
{
    if (debug)
    {
        for (int i = 0; i < 256; i++)
        {
            if (codes[i] != NULL)
            {
                switch (i)
                {
                case '\n':
                    printf("\n'LF'Code: %s\n", codes[i]);
                    break;
                case '\t':
                    printf("'\\t' Code: %s\n", codes[i]);
                    break;
                default:
                    printf("'%c' Code: %s\n", i, codes[i]);
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    // Debug
    char *inFile, *outFile;
    int c;
    int debug;
    inFile = outFile = NULL;
    while ((c = getopt(argc, argv, "i:o:d")) != -1)
    {
        switch (c)
        {
        case 'i':
            inFile = (char *)malloc(sizeof(char) * (strlen(optarg) + 1));
            strcpy(inFile, optarg);
            break;
        case 'o':
            outFile = (char *)malloc(sizeof(char) * (strlen(optarg) + 1));
            strcpy(outFile, optarg);
            break;
        case 'd':
            debug = 1;
            break;
        default:
            break;
        }
    }
    if (inFile == NULL)
    {
        inFile = "completeShakespeare.txt";
    }
    if (outFile == NULL)
    {
        outFile = "huffmanJohnny.out";
    }

    // File pointer
    FILE *currPosition = NULL;
    if (inFile != NULL)
    {
        currPosition = fopen(inFile, "r");
        if (currPosition == NULL)
        {
            fprintf(stderr, "Could not open %s\n", inFile);
            exit(EXIT_FAILURE);
        }
    }

    // 1. Read the input file and use a 256 element array
    // to count the frequency of each character. Debug to
    // print out frequencies

    // Initialize a buffer array to store read data
    charFreq charFreq[256] = {0};
    for (int index = 0; index < 256; index++)
    {
        charFreq[index].ch = index;
        charFreq[index].freq = 0;
    }

    int ASCII; // holds the ASCII value of the character read
    while ((ASCII = fgetc(currPosition)) != EOF)
    {
        charFreq[ASCII].freq++;
    }
    fclose(currPosition);
    debugFrequencies(debug, charFreq);

    // 2. Use the frequencies to build a forest of tree nodes, then
    // sort by frequency and build the huffman tree. Debug to print out
    // total combined frequency
    priorityQueue pq;
    pq.size = 0;
    for (int i = 0; i < 256; i++)
    {
        if (charFreq[i].freq > 0)
        {
            treeNode *node = createNode(charFreq[i].freq, charFreq[i].ch);
            enqueue(&pq, node);
        }
    }
    qsort(pq.items, pq.size, sizeof(treeNode *), compareNodes);
    treeNode *root = buildHuffman(&pq);
    if (debug)
    {
        printf("Total Frequency: %d", combinedFrequency(root));
    }

    // 3. Traverse the tree to collect the binary for each leaf by length
    // and bit sequence. Debug to print out each character with its binary
    char *codes[256] = {0};
    char binary[256];
    traverseHuffmanTree(root, 0);
    collectCodes(root, codes, binary, 0);
    debugPrintCodes(codes, debug);

    // 4. Re-read the file, encode characters, and store bits in a buffer
    // Write a byte with fwrote when the buffer has 8 bits,
    // saving any excess for the next byte
    FILE *out = fopen(outFile, "wb");
    unsigned char buffer = 0;
    int bitCount = 0;

    currPosition = fopen(inFile, "r");
    while ((ASCII = fgetc(currPosition)) != EOF)
    {
        char *binary = codes[ASCII];
        for (int j = 0; binary[j] != '\0'; j++)
        {
            buffer = (buffer << 1) | (binary[j] - '0'); // Convert to corresponding
                                                        // int value so it can be use by bit operators
            bitCount++;

            if (bitCount == 8)
            {
                fwrite(&buffer, sizeof(unsigned char), 1, out);
                buffer = 0;
                bitCount = 0;
            }
        }
    }
    fclose(currPosition); // Close the input file
    if (bitCount > 0)
    {
        buffer <<= (8 - bitCount);
        fwrite(&buffer, sizeof(unsigned char), 1, out);
    }
    fclose(out);

    return 0;
}
