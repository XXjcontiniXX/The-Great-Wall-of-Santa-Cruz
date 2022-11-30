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
static uint8_t bufbuf[256];
static uint32_t woffset = 0;
static uint32_t last_byte = 0;
static uint32_t offset = 0; 
static uint32_t wdiff = 0;
uint32_t loffset = 0;

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
	loffset = woffset;
	if (!(woffset >= BLOCK * 8)) { // woffset cannot be 32768 or above
		while (code_pop_bit(c, &bit)) { // stop when a code is empty
			wbuffer[loffset/8] = wbuffer[loffset/8] | (bit << (7 - loffset % 8));
			loffset--;
		}
		woffset++;
	}else{
		printf("once");
		wdiff = loffset - ((BLOCK * 8) - 1); // will always be positive
		uint8_t i = code_size(c) - 1;
		while (loffset > (BLOCK * 8) - 1 && code_pop_bit(c, &bit)) { // copy backend which doesn't fit into buffer into bufbuf
			printf("twice\n");
			bufbuf[i/8] = bufbuf[i/8] | (bit << (7 - i % 8)); // but when loffset finally reaches within the range of the block
			i--;
			loffset--; // then end loop
		}
		while (code_pop_bit(c, &bit)) { // pop remaining bits into actual buffer
			printf("never happen\n");
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
	//uint8_t block = 0;
	if (woffset > BLOCK * 8) {
		printf("woffset = %u\n", woffset);
		write_bytes(outfile, wbuffer, (loffset / 8) + 1); // woffset is in the next byte, so go back one							
		while (wdiff % 8 != 0) {
			bufbuf[wdiff / 8]  = bufbuf[wdiff / 8] | (byte << (7 - (wdiff % 8)));
			wdiff++;
		}
		write_bytes(outfile, bufbuf, (wdiff / 8) + 1);
		//woffset = 0;
		//loffset = 0;

	}
	/*	     //	                      |<--here, the woffset increase
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
	//printf("woffset flush = %u\n", woffset);
	//write_bytes(outfile, wbuffer, ((woffset - 1) / 8) + 1);
	*/
	woffset = 0;
	return;
}
