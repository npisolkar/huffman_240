//File for functions focused on building a Huffman Tree
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"



/**
 * Opens the specified file and counts the frequency of each ascii character
 * populates ascii_freq with the frequencies
 * uses intializeNodeArray to allocate memory for node array
 * sorts the node array
 * returns a pointer to the fully populated, sorted NodeArray for that file
 */
NodeArray * createHistogramFromFile(char * input_file, int * ascii_freq){	
	// needs to open the file
  FILE * fp = fopen(input_file, "r");
  if (fp == NULL) {
    printf("unabe to open file\n");
    return NULL;
  }

  // then populate the ascii_freq arrray while updating nChars
  int nChars = 0;

  while (!feof(fp)) {
    char c = fgetc(fp);
    if (feof(fp)) break;

    if (c >= 0 && c <= 127) { //128
      if (ascii_freq[c] == 0) {
        nChars++; //increments nChars for new characters
      }
      ascii_freq[c] += 1; //adds to the histogram
    }
  }

  //than use that information to initializeNodeArray() and return that pointer
  NodeArray * node_array = initializeNodeArray(ascii_freq, nChars);

  sortNodeArray(node_array);
  
  //printf("After sorting\n");
  //printNodeArray(node_array);

  return node_array;
}

/** 
 * allocates memory for a node of value c and freq freq
 * and adds it to the NodeArray node_array 
 */
void insertInNodeArray(int c, int freq, NodeArray * node_array){
  // First calls resizeArrayIfNeeded
  //resizeArrayIfNeeded(node_array);
  
  //printf("adding char: %c of freq %d, nChars currently %d\n", c, freq, 
    //node_array->nChars);

  if (node_array == NULL) return;

  //creates new_node 
  node_array->nodes[node_array->nChars] = (Node *) malloc(sizeof(Node));
  Node * new_node = node_array->nodes[node_array->nChars];
  
  if (new_node == NULL) return;

  //populates node
  new_node->left = NULL;
  new_node->right = NULL;
  new_node->val = c;
  new_node->freq = freq;

  //adds node to array and increment nChars
  //node_array->nodes[node_array->nChars] = new_node;
  node_array->nChars = node_array->nChars + 1;
}


// Node comparison function
// Compares two nodes by frequency
//
// Returns the difference a->freq - b->freq if it is non-zero
// Else, returns the difference a->val - b->val
int compNodes (const void * a, const void * b){
	Node * anode = (Node *) a;
  Node * bnode = (Node *) b;

  //istn is supposed to be frequency??
  int diff = anode->freq - bnode->freq;
  if (diff != 0) return diff;

  return anode->val - bnode->val;
}

// Returns the minimum node of the node_array,
// assuming that the array has been sorted
// If the array is empty, returns NULL
Node * getMin(NodeArray * node_array){
	if (node_array->nChars > 0) 
		return (Node*)node_array->nodes;
	else return NULL;
}

//Selects the minimum of two nodes as the next child based on frequency
//Character value is used as a tie breaker if needed
Node* selectChild(Node* n1, Node* n2){
	//: write your implemeintation here
}

//Initializes a parent node by assigning its children
//Also sets its frequency to the sum of its children's frequencies
//and sets its value to the minimum value of its children
void initParent(Node* parent, Node* left, Node* right){
	parent->freq = left->freq + right->freq;
	parent->left = left;
	parent->right = right;
	
  if(right->val < left->val) {
    parent->val = right->val;
  } else {
    parent->val = left->val;
  }
}

/**
 * Sorts the nodearray using qsort function 
 */
void sortNodeArray(NodeArray * node_array){
	// Ascending sorting function for the nodes of the node array
  // Hint: use the function compNodes to simplify your function
  if (node_array == NULL) return;
  //int (*comp_func)(const void *, const void *) = &compNodes;
  //qsort(node_array->nodes, node_array->nChars, sizeof(Node), comp_func);


  for (int i =  node_array->nChars - 2; i > 0; i--) {
    for (int j = 0; j <= i; j++) {
      Node *n1 = node_array->nodes[j];
      Node *n2 = node_array->nodes[j + 1];
      if (compNodes(n1, n2) > 0) {
        node_array->nodes[j+1] = n1;
        node_array->nodes[j] = n2;
      }
    }
  }
}

/** 
 * gets passed a populated int * ascii freq and 
 * number of non-zero characters
 * Allocates, initializes, and returns a node array for those parameter
 */
NodeArray * initializeNodeArray(int * ascii_freq, int nChars){
  //needs to take the int 8 ascii_freq  and createnodes 
  // for all of its non-zero values and create a node for each one 
  // of them, order not mattering
  if (ascii_freq == NULL) return NULL;
  NodeArray * new_array = (NodeArray *) malloc(sizeof(NodeArray));
  //new_array->capacity = nChars;
  
  new_array->nChars = 0; 
  Node ** node_adress = (Node **) malloc(sizeof(Node *) * 127);
  new_array->nodes = node_adress;
  
  for (int i = 0; i < 127; i++) {
    if (ascii_freq[i] != 0) {
      insertInNodeArray(i, ascii_freq[i], new_array);
    }
  }
  
  //printf("after initialization\n");
  //printNodeArray(new_array);

  return new_array;
}

/*
// Checks to see if nChars == capacity in the node_array
// If so, the size of the node_array is doubled
void resizeArrayIfNeeded(NodeArray * node_array){
  if (node_array == NULL) return;

  if (node_array->nChars < node_array->capacity) return;

  //double hte memory size of node array
  node_array->capacity *= 2;
  *(node_array->nodes) = realloc(*(node_array->nodes), capacity * (sizeof(Node)));
  //is this logic correct, nodes is a pointer to the array of nodes
}
*/

// Prints the node array in the format "[character], [frequency]\n"
void printNodeArray(NodeArray * node_array){
	for (int i=0; i < node_array->nChars; i++){
		//Node * n = (Node*)((char*)node_array->nodes + i*sizeof(Node));
		printf("%c, %d\n", node_array->nodes[i]->val, node_array->nodes[i]->freq);
	}
}

/*
 * Free memory allocated to the node array
 */
void clearNodeArray(NodeArray * node_array){
  for (int i = 0; i < node_array->nChars; i++) {
    free(node_array->nodes[i]);
  }
  free(node_array->nodes);
  free(node_array);
}


// Builds a Huffman Tree and associates characters to their code using a node_array
// Assumes that the node array has already been created and sorted
NodeArray * buildHuffmanTree(NodeArray * node_array) {
	// contins all internal nodes
  NodeArray * huffman_tree;
  int huff_len = node_array->nChars - 1;
  
  huffman_tree = (NodeArray *) malloc(sizeof(NodeArray));
	if (huffman_tree == NULL) {
    printf("malloc error 201\n");
  }
  huffman_tree->nChars = huff_len;
  huffman_tree->nodes = (Node **) malloc(sizeof(Node *) * huff_len);
  
  
  //Declare indicies for both arrays
	int next_leaf = 0, next_int = -1; // leaf index available and internal 
	//Declare the next available index to use for internal nodes
	//int next_internal = 0;

  Node ** arr_nodes = node_array->nodes;
  Node ** huff_nodes = huffman_tree->nodes;

  Node * left;
  Node * right; //placeholders
	for (int i = 0; i < huff_len; i++) {
    //for index i in huffman_tree, create internal node
    huffman_tree->nodes[i] = (Node *) malloc(sizeof(Node));  
    Node * parent = huffman_tree->nodes[i];
    
    // if the node_array available is less than the huffman variable
    // make left the node array
    left = arr_nodes[next_leaf];
    if (next_int != -1) right = arr_nodes[next_int];
    if (next_int < 0) {
      initParent(parent, left, arr_nodes[next_leaf + 1]);
      
      //printf("initparent index: %d; left freq: %d right freq %d\n ",
      //        i, left->freq, arr_nodes[next_leaf + 1]->freq);
      
      next_leaf += 2;
      next_int++;
      continue;
    }
    
    //set left
    
    if (arr_nodes[next_leaf] == NULL) {
      left = huff_nodes[next_int];
      next_int++;
    } else if (compNodes(arr_nodes[next_leaf], huff_nodes[next_int]) < 0) {
      //next histogram  node is less than next internal node
      left = arr_nodes[next_leaf];
      next_leaf++;
    } else {
      left = huff_nodes[next_int];
      next_int++;
    }
    //set right
    if (arr_nodes[next_leaf] == NULL) {
      right = huff_nodes[next_int];
      next_int++;
      //right = huff_nodes[next_int];
    } else if (compNodes(arr_nodes[next_leaf], huff_nodes[next_int]) < 0) {
      //next histogram  node is less than next internal node
      right = arr_nodes[next_leaf];
      next_leaf++;
    } else {
      right = huff_nodes[next_int];
      next_int++;
    } 
    initParent(parent, left, right); //makes all of the stuff for the thing
    //printf("initparent index: %d; left freq: %d right freq %d\n ",
    //          i, left->freq, right->freq);
  }

  // iterate through node_array and any created internal nodes to build the huffman tree:
	// (1) While any leaves or more than 1 non-leaves have not been turned into children, 
	// 		select the minimum two nodes from either node array
	// (2) Use these nodes as the children left and right passed to initParent
	// (3) Be sure to manage your indicies appropriately!
	
	//Assign the codes for the leaves (characters) of the Huffman Tree and then return it
	codifyChars(huffman_tree);
	return huffman_tree;
}


//Returns the root node of the Huffman Tree
Node * getRootHuffmanTree(NodeArray * huffman_tree){
	//return (Node*) ((char*)huffman_tree->nodes + (huffman_tree->nChars-1)*sizeof(Node));
  return huffman_tree->nodes[huffman_tree->nChars - 1];
}

// Checks if a node is a leaf node or not
// A leaf node has no children (so its left and right pointers are NULL)
//
// Returns 1 if the node is a leaf, 0 otherwise
int isLeafNode(Node * n){
	if (n->left == NULL && n->right == NULL) return 1;
	else return 0;
}

// Prints a huffman tree recursively from the root
// Prints a node as "[ascii value] = [ascii character]\t[bitstring]]\n"
// Note: The ascii value should be padded with 0's to be 3 digits
// Note2: The left subtree is assigned a 1 bit, 
// 		and the right subtree is assigned a 0 bit
void printTree(Node * root, int code[], int i){
	if (root == NULL) return;	

  if (isLeafNode(root)) {
  printf("%03d = %c\t", root->val, root->val);
	for (int m = 0; m < i; m++) {
    printf("%u", code[m]);
  }
  printf("\n");
  }

	//Assign codes for the left subtree
	code[i] = 1;
	printTree(root->left, code, i+1);

	//Assign codes for the right subtree
	code[i] = 0;
	printTree(root->right, code, i+1);
}

// Traverses a Huffman Tree from the root to each node, and records each path
// As the code string for each character
//
// The left child of a node should have a 1 bit appended to their code
// The right child of a node should have a 0 bit appended to their code
void textToCode(Node * root, int code[], int i){
	if (root == NULL) return;

  if (isLeafNode(root)) {
    for (int m = 0; m < i; m++) {
      char_to_code[root->val][m] = code[m] + '0';
    }
    char_to_code[root->val][i] = '\0';
  }
	
	//Assign codes for the left subtree
	code[i] = 1;
	textToCode(root->left, code, i+1);

	//Assign codes for the right subtree
	code[i] = 0;
	textToCode(root->right, code, i+1);
}

// Prints a huffman tree by calling printTree on the root node of the tree
void printHuffmanTree(NodeArray * huffman_tree){
	Node * root = getRootHuffmanTree(huffman_tree);

	int code[MAXCODELENGTH] = {0};
	printTree(root, code, 0);
}

// Calls textToCode on the root of the provided huffman tree
void codifyChars(NodeArray * huffman_tree){
	Node * root = getRootHuffmanTree(huffman_tree);
  
  for (int i = 0; i < 256; i++) {
    char_to_code[i] = (char *) malloc(MAXCODELENGTH);
  }

	int code[MAXCODELENGTH] = {0};
	textToCode(root, code, 0);	
}

void clearCTC() {
  for (int i = 0; i < 256; i++) {
    free(char_to_code[i]);
  }
}
