//File for functions focused on compression/encoding
#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"
#include <string.h>

// Writes the number of unique characters to the file as a single byte
// Then, for each character in the node array, write the character (as a byte)
// and the frequency of that character (size = sizeof(unsigned int))
//Format: [nChars] [char1 ascii code(char)] [char1 frequency(int)] [char2 ascii code] [char2 frequency] ...
void writeCharFrequenciesinFile(FILE * out_file, NodeArray * node_array){
	fwrite(&node_array->nChars, sizeof(int), 1, out_file);
  for ( int i = 0; i < node_array->nChars; i++) {
    fwrite(&node_array->nodes[i]->val, sizeof(char), 1, out_file);
    fwrite(&node_array->nodes[i]->freq, sizeof(unsigned), 1, out_file);
  }
}

static int nbits;
static unsigned char buffer;

// Adds 0 bits to the end of the final byte if the encoded document
// ends with a partial byte (cannot write single bits to the file!)
void padBuffer(FILE * out_file){
	for (int i = nBits; i < 8; i++) {
    writeBitBuffer(0, out_file);
  }
}

// Add the next bit to the bit buffer. If a full byte is buffered, 
// write it to the file (cannot write individual bits to a file!)
// Reset nBits and the bitBuffer if the buffer is written to the file
void writeBitBuffer(int bit, FILE * out_file){
	buffer = (buffer | (bit << (7-nbits)));
  nbits++;
  if (nbits == 8) {
    fwrite( &buffer, sizeof(char), 1, out_file);
    nbits = 0;
    buffer = 0x0; 
  }
}

// Given a code string, it takes each "bit" representing character and
// writes it to the bit buffer for the output file
void writeCodeToFile (char * code, FILE * out_file){
  for (int i = 0; i < strlen(code); i++) {
    writeBitBuffer(code[i] - '0', out_file);
  }
}


// Primary compression method
// (1) stores frequencies in the output file
// (2) rewrites the input file using the Huffman Code in the huffman_tree
void createCompressedFile(char* input_file, char * output_file, NodeArray * node_array, NodeArray * huffman_tree){
	if (input_file == NULL || output_file == NULL) return;

  FILE * fp_in = fopen(input_file, "r");
  FILE * fp_out = fopen(output_file, "w");

  writeCharFrequenciesinFile(fp_out, node_array);

  char c = 0;
  while (!feof(fp_in)) {
    c = fgetc(fp_in);
    if (c >= 0 && c <= 127) writeCodeToFile(char_to_code[c], fp_out);
  }
  padBuffer(fp_out);
  fclose(fp_in);
  fclose(fp_out);
}



















