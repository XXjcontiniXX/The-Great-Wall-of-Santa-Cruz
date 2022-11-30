#include "code.h"
#include "io.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

uint64_t bytes_read = 0;
uint64_t bytes_written = 0;
static uint8_t buffer[BLOCK];
static uint8_t wbuffer[BLOCK];
static uint32_t woffset = 0;
static uint32_t last_byte = 0;
static uint32_t offset = 0; 

int read_bytes(int infile, uint8_t *buf, int nbytes) {
	int i = 0;
	int j;
	while (nbytes - i > 0 && (j = read(infile, buf + i, nbytes - i)) > 0) {
		i += j;
	}
	bytes_read += i;
	return i;	
}

int write_bytes(int outfile, uint8_t *buf, int nbytes) {
	int i = 0;
	int j;
	while (nbytes - i > 0 && (j = write(outfile, buf + i, nbytes - i)) > 0) {  // when does read() not read the full block
                i += j;
        }
        bytes_written += i;
        return i;
}

bool read_bit(int infile, uint8_t *bit) {
	uint8_t byte = 1;
	if (offset >= BLOCK * 8 || offset == 0) { // 
		last_byte = read_bytes(infile, buffer, BLOCK); // bufdex cannot be more than 4096
		offset = 0;
		if (last_byte == 0) {
			return false;
		}
	}
		
	
	*bit = (bool)(buffer[offset/8] & (byte << (7 - offset % 8)));
	offset++;
	
	if (last_byte < 4096) { // if we are only reading up to test
                if (offset / 8 == last_byte - 1) {
                        return false;
                }
        }
	return true;
}

void write_code(int outfile, Code *c) {
	uint8_t bit = 0;
	if (code_empty(c)) { // if code size of c is 0 then quit
		return;
	}
	
       	woffset += code_size(c) - 1;
	printf("%u.", woffset);
	uint32_t loffset = woffset;
	if (!(woffset >= BLOCK * 8)) {
		while (code_pop_bit(c, &bit)) { // stop when a code is empty
			wbuffer[loffset/8] = wbuffer[loffset/8] | (bit << (7 - loffset % 8));
			loffset--;
		}
		woffset++;
	}else{
		printf("once");
		uint8_t bufbuf[256];
		while (loffset > BLOCK - 1 && code_pop_bit(c, &bit)) { // copy backend which doesn't fit into buffer into bufbuf
			bufbuf[loffset/8] = bufbuf[loffset/8] | (bit << (7 - loffset % 8)); // but when loffset finally reaches within the range of the block
			loffset--; // then end loop
		}
		while (code_pop_bit(c, &bit)) { // pop remaining bits into actual buffer
                        wbuffer[loffset/8] = wbuffer[loffset/8] | (bit << (7 - loffset % 8));
                        loffset--;
                }
		flush_codes(outfile); // flush and write that buffer
			
	}
	/*
	printf("%u\n", woffset);
	if (woffset > BLOCK * 8) {
		flush_codes(outfile);
		printf("hey here once");
		if (loffset > 0) { // if the buffer is full it'll flush the whole thing but if there is more bits in code
			printf("not here tho");
			woffset = loffset; // set the woffset to loffset which is how many bits r left
			while (code_pop_bit(c, &bit)) { // pop the rest off
                		wbuffer[loffset/8] = wbuffer[loffset/8] | (bit << (7 - loffset % 8));
                		loffset--;
        		}
        		woffset++;
		}

	}
	*/
	return;
}

void flush_codes(int outfile) {
	uint8_t byte = 0;
	uint8_t block = 0;
	if (woffset > BLOCK * 8) {
		printf("woffset = %u\n", woffset);
		write_bytes(outfile, wbuffer, ((woffset - 1) / 8) + 1); // woffset is in the next byte, so go back one
		woffset = 0;
		return;
	}
						     //	                      |<--here, the woffset increase
	if (woffset % 8 == 0) {
		block = (woffset - 1) / 8;
	}else{
		block = woffset / 8;
	}
	while (woffset / 8 == block) { // while woffset is in the byte, zero then increment
		wbuffer[block] = wbuffer[block] | (byte << (7 - woffset % 8));
		woffset++;
		printf("happen 8 times please\n");
	}
	printf("woffset flush = %u\n", woffset);
	write_bytes(outfile, wbuffer, ((woffset - 1) / 8) + 1);
	woffset = 0;
	return;
}
