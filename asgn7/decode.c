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
	
	if (v > 1) {
                fprintf(stderr, "%s", help);
                return 0;
        }
	
	// going to read in bytes from infile into the header
	union {
                uint8_t bytes[sizeof(Header)];
                Header h;
        } hu;
	
	read_bytes(infile, hu.bytes, sizeof(Header));
	
	printf("hu.b[0]: %u\n", hu.bytes[0]);
	
			
	
	
	
	return 0;
}
