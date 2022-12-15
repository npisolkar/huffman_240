# huffman_240
Huffman text file compressio tool written in C

After using typing [make] command, run [huffman] followed by tags and files
to compress/decompress files

p: print huffman of non-encoded file
pc: print huffman of encoded file (recontuct it from the header data)
pn: print node array of non-encoded file                                                             
e: encode infile into outfile            
	-ep: encode and print the huffman tree
d: decode coded file into outfile        
	-dpc: decode and print the huffman tree    

huffman [-pna infile] [-ph uncoded_file] [-phc coded_file] [-e infile outfile] [-d coded_file outfile]compress.c
   

huffman.c
  main program with instructions for use and main definition

tree.c
  contains implementation for creation of tree creation from files

compress.c 
  


