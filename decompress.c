//File for functions focused on decompression/decoding
#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

unsigned int offset;
unsigned int totalChars;
// Read and return a single int from the file
int readIntFromFile(FILE * in_file){
	int j;
  fread(&j, sizeof(int), 1, in_file);
  return j;
}


// Read and return a single char from the file
char readCharFromFile(FILE * in_file){
	return (char)fgetc(in_file);
}

// Read and return the number of unique characters in the file
// Obtain all character frequencies of the encoded file and store them in ascii_freq
int loadFrequenciesFromFile(FILE * in_file, int* ascii_freq){
	fseek(in_file, 0, SEEK_SET);
  int n = readIntFromFile(in_file);
  offset = sizeof(int) + n * (sizeof(char) + sizeof(unsigned));
  totalChars = 0;
  for (int i = 0; i < n; i++) {
    char c;
    int f;
    fread(&c, sizeof(char), 1, in_file);
    fread(&f, sizeof(unsigned), 1, in_file);
    offset = ftell(in_file);
    if (c >= 0 && c <= 127) {
      ascii_freq[c] = f;
      totalChars += f;
    }
  }
  return n;
}


// Allocates, initializes, and returns a node array from the file
// (1) Load all frequency information from the header of the specified file
// (2) Builds a node array from the file frequencies
NodeArray * buildNodeArrayFromFile(FILE* in_file){
	if (in_file == NULL) return NULL;
  NodeArray * node_array;
  int ascii_freq[256] = {0};
  int nChars = loadFrequenciesFromFile(in_file, ascii_freq);
  node_array = initializeNodeArray(ascii_freq, nChars);
  return node_array;
}

// Decodes the input file using the huffman tree
// Write the decoded file into the output file
// Assumes that the next byte to read is the first byte to decode (just past the header)
void writeUncompressedFile(FILE* in_file, FILE* out_file, NodeArray* huffman_tree){
  // for loop takes each byte read into fgetc
    //go through each bit << to traverse the huffman tree and at each leaf
    //reinstate at the getroot of huffman_tree
  
  //printHuffmanTree(huffman_tree);

  //codifyChars(huffman_tree);
  int currentChars = 0;
  if (in_file == NULL || out_file == NULL|| huffman_tree == NULL) {
    printf("null pointer exception");
  }
  
  //printHuffmanTree(huffman_tree);

  unsigned char c;
  Node * node = huffman_tree->nodes[huffman_tree->nChars - 1];
  fseek(in_file, offset, SEEK_SET);
  while (!feof(in_file)) {
    fread(&c, sizeof(char), 1, in_file);
   // c = readCharFromFile(in_file);
    for (int i = 0; i < 8; i++) {
      if ((c >> (7 - i)) % 2 == 0) {
        //printf("0");
        node = node->right;
      } else if ((c >> (7 - i)) % 2 == 1) {
        node = node->left;
        //printf("1");
      }
      if (node->left == NULL && node->right == NULL) { // if at a leaf node
        currentChars++;
        if (currentChars <= totalChars) {
          fprintf(out_file, "%c", node->val);
        } // reset node and print
        node = getRootHuffmanTree(huffman_tree);       // to out_file
      }
    }
  }
}

// Primary decoding function: 
// (1) Open the input file and load all frequency information
// (2) Build the Huffman Tree for those frequencies
// (3) Traverse the Huffman Tree bit-by-bit and write each decoded
// character to the output file
// (4) Free the node arrays and close the files
void uncompress(char * input_file, char * output_file){
  //if (input_file == NULL || output_file == NULL) returni;

  FILE * in_f = fopen(input_file, "r");
  FILE * out_f = fopen(output_file, "w");

  
  NodeArray * node_array = buildNodeArrayFromFile(in_f);
  sortNodeArray(node_array);
  NodeArray * huffman_tree = buildHuffmanTree(node_array);

  writeUncompressedFile(in_f, out_f, huffman_tree);

  fclose(in_f);
  fclose(out_f);
  clearNodeArray(node_array);
  clearNodeArray(huffman_tree);
  clearCTC();
}


















