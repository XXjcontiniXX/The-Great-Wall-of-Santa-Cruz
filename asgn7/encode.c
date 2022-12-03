#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "node.h"
#include "pq.h"
#include "code.h"
#include "io.h"
#include "huffman.h"
#include "header.h"
#include "stack.h"
#include <errno.h>
#define OPTIONS "vho:i:" 

int main(int argc, char **argv) {
	char help[] = "SYNOPSIS\n  A Huffman encoder.\n  Compresses a file using the Huffman coding algorithm.\n\nUSAGE\n  ./huff-encode [-h] [-i infile] [-o outfile]\n\nOPTIONS\n  -h             Program usage and help.\n  -v             Print compression statistics.\n  -i infile      Input file to compress.\n  -o outfile     Output of compressed data.\n";
	uint8_t v = 0;
	int infile = 0;
	int outfile = 1;
	int opt = 0;
	//static uint64_t bytes_written = 0;
	//static uint64_t bytes_read = 0;
	while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
		switch (opt) {
	        	case 'v':
	          		v = v | (1 << 0);
        			break;
			case 'h':
		        	v = v | (1 << 1);
				break;
			case 'o':
				
				if ((outfile = open(optarg, O_WRONLY)) < 0) {
					outfile = open(optarg, O_WRONLY | O_CREAT);
				}

				break;
			case 'i': 
				infile = open(optarg, O_RDONLY);
				break;
			default:
				fprintf(stderr, "%s", help);
				return 1;
			break;
			
		}
	}


	if (v == 2) {
		fprintf(stderr, "%s", help);
		return 0;
	}
	
	// Making a tmpfile before everything to read for the code emitting
		int tmpdes = open("tmpdes.txt", O_RDWR | O_CREAT);
	//
	
	// initialized & zeroes histogram
	uint64_t hist[ALPHABET];
	for (uint32_t i = 0; i < ALPHABET; i++) {
		hist[i] = 0;
	}
	//
	
	// reads one byte at a time from infile
	uint8_t character[1];
	uint64_t bytes = 0;
	while (read_bytes(infile, character, 1)) { // while there are bytes in the array bufferof[char] should be incremented.
		bytes++;
		bytes_written += write_bytes(tmpdes, character, 1);
		hist[character[0]] += 1;
	}
	//
	
	// if the first and second symbol are zero make it them one
	if (hist[0] == 0) {
		hist[0] = 1;
	}
	if (hist[1] == 0) {
		hist[1] = 1;
	}
	//
	
	// builds tree and returns root
	Node *root = build_tree(hist);
	//
	
	// builds the codes
	Code table[ALPHABET];
	build_codes(root, table);
	//code_print(&table[105]);
	//
	
	// constructs a header containing metadata essentially
	uint16_t tree_size = 0;
	for (uint32_t i = 0; i < ALPHABET; i++) {
		if (hist[i] > 0) {
			tree_size++;
		}
	}
	tree_size = (tree_size * 3) - 1;
	///
	
	struct stat *file_stat; // allocate space for stat pointer
	file_stat = malloc(sizeof(struct stat));
	uint64_t file_size = 0;
	if (file_stat == NULL) {
		fprintf(stderr, "encode: [ERROR] malloc failed to allocate memeory for the file stat.\n");
		return 1;
	}

	fstat(infile, file_stat); // put infile's metadata in file_stat
	if (infile != 0) {
		file_size = file_stat->st_size; // st_size contains the size in bytes
	}else{
		file_size = bytes;
	}
	//printf("file_size: %lu\n", file_size); 
	uint16_t permissions = file_stat->st_mode;
	//printf("permissions %u\n", permissions);
	
	free(file_stat);
	file_stat = NULL;

	if (fchmod(outfile, permissions) < 0) { // set outfiles permissions
		fprintf(stderr, "encode: [ERROR] outfile's permissions setting failed...err code: %u\n", errno);
		return 1;
	}
	///
	
	// union creation
	union {
    		uint8_t bytes[sizeof(Header)];
    		Header h;
	} hu;
	
	(hu.h).magic = MAGIC;
	(hu.h).permissions = permissions;
	(hu.h).tree_size = tree_size;
	(hu.h).file_size = file_size;

	write_bytes(outfile, hu.bytes, sizeof(hu)); // writes to outfile, the header casted to uint8_t for the entire size of header
	
	// dump the tree to outfile
	dump_tree(outfile, root);
	// character[] was inited earlier
	lseek(tmpdes, 0, SEEK_SET);	
	while (read_bytes(tmpdes, character, 1)) {
		//printf("%u,", character[0]);
		Code c = table[character[0]];
		write_code(outfile, &c); // write code pops code off of the code

	}
	flush_codes(outfile); // flushhhhhh

	if (infile != 0) {
		close(infile);
	}
	if (outfile != 1) {
		close(outfile);
	}
	remove("tmpdes.txt");
	// then destroy tree
	// destroy pq
	// destroy stack
	return 0;
}
