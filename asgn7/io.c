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
static bool  rfinished = false;
uint32_t loffset = 0;

int read_bytes(int infile, uint8_t *buf, int nbytes) {
	int i = 0;
	int j;
	//int prev = 0;
	while (nbytes - i > 0 && (j = read(infile, buf + i, nbytes - i)) > 0) {
		//prev = j;
		i += j;
	}

	/*if (prev < 4096) { // and j = 0 which it must
		i--;
	}*/
	bytes_read += i;
	return i;	
}

int write_bytes(int outfile, uint8_t *buf, int nbytes) {
	int i = 0;
	int j;
	//printf("buf[0] = %u nbytes = %u\n", buf[0], nbytes);
	
	while (nbytes - i > 0 && (j = write(outfile, buf + i, nbytes - i)) > 0) {
        	i += j;
		printf("j = %u\n", j);
        }
		
        bytes_written += i;
        return i;
}

bool read_bit(int infile, uint8_t *bit) {
	uint8_t byte = 1;
	if (rfinished) {
                return false;
        }
	if (offset >= BLOCK * 8 || offset == 0) { // 
		last_byte = read_bytes(infile, buffer, BLOCK); // bufdex cannot be more than 4096
		//printf("last_byte = %u\n", last_byte);
		/*
		if (last_byte < 5) {
			for (uint8_t i = 0; i < last_byte; i++) {
				printf("buffer[%u] = %u\n", i, buffer[i]); 
			}
		} */
		offset = 0;
		if (last_byte == 0) {
			return false;
		}
	}
		
	
	*bit = (bool)(buffer[offset/8] & (byte << (7 - offset % 8)));
	offset++;
	
	if (last_byte < 4096) { // if we are only reading up to test
                if (offset / 8 == last_byte) {
			rfinished = true;
                }
        }
	return true;
}

void write_code(int outfile, Code *c) {
	uint8_t bit = 0;
	uint8_t one = 1;
	if (code_empty(c)) { // if code size of c is 0 then quit
		return;
	}
	/*if (c == 32511) {
		printf("woffset: %u\n", woffset);
	}*/
       	woffset += code_size(c) - 1;
	//printf("%u.", woffset);
	loffset = woffset;
	
	if (!(woffset >= BLOCK * 8)) { // woffset cannot be 32768 or above
		while (code_pop_bit(c, &bit)) { // stop when a code is empty
			if (bit == 1) {
				wbuffer[loffset/8] = wbuffer[loffset/8] | (bit << (7 - loffset % 8));
			}else{
				wbuffer[loffset/8] = wbuffer[loffset/8] &  ~((one << (7 - loffset % 8)));    // 0001 0000 --> 1110 1111 & 
			}
			
			loffset--;
		}
		woffset++;

		if (woffset == BLOCK * 8) {
			printf("three times\n");
			flush_codes(outfile);
		}
	}else{
		printf("woffset in splitt: %u\n", woffset);
		wdiff = loffset - ((BLOCK * 8) - 1); // will always be positive
		//uint8_t i = code_size(c) - 1;
		wdiff -= 1;
		uint8_t i = wdiff;
		// change right here////////////////
		//printf("just a test\n");
		while (loffset > (BLOCK * 8) - 1 && code_pop_bit(c, &bit)) { // copy backend which doesn't fit into buffer into bufbuf	
			//printf("8 times\n");
			//
			if (bit == 1) {
                                bufbuf[i/8] = bufbuf[i/8] | (bit << (7 - i % 8));
                        }else{
                                bufbuf[i/8] = bufbuf[i/8] &  ~((one << (7 - i % 8)));    // 0001 0000 --> 1110 1111 &
                        }

			//
			//bufbuf[i/8] = bufbuf[i/8] | (bit << (7 - i % 8)); // but when loffset finally reaches within the range of the block
			//printf("bufbuf[%u//8]: %u\n", i/8, bufbuf[i/8]);
			
			//for (uint8_t i = 0; i < 8; i++) {
                          //     printf("%u\n", (bool)(bufbuf[i/8] & (bit << (7 - i % 8))));
                        //}
			//printf("\n");
			i--;
			loffset--; // then end loop
		}

		while (code_pop_bit(c, &bit)) { // pop remaining bits into actual buffer
			//printf("bit: %u chunk: %u index (0 is on right): %u loffset: %u\n", bit, loffset / 8, (7 - loffset % 8), loffset);
                       if (bit == 1) {
                                wbuffer[loffset/8] = wbuffer[loffset/8] | (bit << (7 - loffset % 8));
                        }else{
                                wbuffer[loffset/8] = wbuffer[loffset/8] &  ~((one << (7 - loffset % 8)));    // 0001 0000 --> 1110 1111 &
                        }
			/*for (uint8_t i = 0; i < 8; i++) {
				printf("wbuffer: %u\n", wbuffer[loffset/8] | (bit << (7 - loffset % 8)));
			}*/
			//printf("\n");
                        loffset--;
                }
		flush_codes(outfile); // flush and write that buffer
			
	}
	return;
}

void flush_codes(int outfile) {
	uint8_t one = 1;
	//uint8_t block = 0;
	//printf("happens after wbuffer\n");
	if (woffset > BLOCK * 8) {
		printf("flush() woffset > BLOCK * 8\n");
		//printf("woffset = %u\n", woffset);
		write_bytes(outfile, wbuffer, BLOCK); // woffset is in the next byte, so go back one							
		while (wdiff % 8 != 0) {
			bufbuf[wdiff / 8]  = bufbuf[wdiff / 8] & ~(one << (7 - (wdiff % 8)));
			wdiff++;
			printf("None or < 8 times\n");
		}
		//printf("bufbuf[0]: %u", bufbuf[1]);
		
		write_bytes(outfile, bufbuf, (wdiff / 8));
		//woffset = woffset - (BLOCK * 8) + 1;
		woffset = 0;

	}else if (woffset == BLOCK * 8) { 
		printf("flush woffset == BLOCK *8\n");
		write_bytes(outfile, wbuffer, BLOCK);
		woffset = 0;
	}else{
		if (woffset == 0) {
			return;
		}
		printf("flush() woffset < BLOCK * 8\n");
		printf("woffset: %u\n", woffset);
		//printf("woffset for normal should be 128: %u\n", woffset);
		uint32_t tmp_woffset = woffset;
		while (tmp_woffset % 8 != 0) {
			printf("wbuffer val: %u\n", wbuffer[tmp_woffset / 8]);
			wbuffer[tmp_woffset / 8]  = wbuffer[tmp_woffset / 8] & ~(one << (7 - (tmp_woffset % 8)));
			tmp_woffset++;
			printf("5 times\n");
		}
		// woffset might need to be woffset += 1;
		//woffset += 1;
		//printf("wbuffer[0] %u wbuffer[1] %u\n", wbuffer[0], wbuffer[1]);
		write_bytes(outfile, wbuffer, ((woffset - 1) /8) + 1); // woffset never greater than 326
	
	}

	// TODO dont forget about situation where only a couple codes have been put in that means 
	// we should write starting at woffset to the end of that byte and then writebytes at woffset
	// unless woffset % 8 because then the byte we want is full and its in a new one
	return;
}
