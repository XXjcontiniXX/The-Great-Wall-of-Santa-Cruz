#include "code.h"
#include "io.h"
#include "string.h"
#include <stdbool.h>
#include <stdlib.h>
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
static uint32_t stoffset = 0;
static uint32_t last_byte = 0;
static uint32_t offset = 0; 
static bool rfinished = false;
static bool first_call = true;

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

        if (code_empty(c)) {
                return;
        }
	
	if (first_call == true) {
		first_call = false;
		woffset += code_size(c) - 1; // stoffset = stoffset - 1 only once
	}else{
		woffset += code_size(c); // woffset always on bit that has been changed  // so in any case: woffset + 1 is an
	}

	stoffset = woffset;
	if (woffset < 32768) {
       		while (code_pop_bit(c, &bit)) {
                	if (bit == 1) {
                                	wbuffer[stoffset/8] = wbuffer[stoffset/8] | (bit << (7 - stoffset % 8));
                        	}else{
                                	wbuffer[stoffset/8] = wbuffer[stoffset/8] &  ~((one << (7 - stoffset % 8)));    // 0001 0000 --> 1110 1111 &
                        	}
			stoffset = stoffset == 0 ? 0 : stoffset - 1;
        	}
		// IFF (if and only if)
		if (woffset == 32767) { // the bit which has been changed is the last bit flush it
			flush_codes(outfile);
		}
	}else{ // if there is an overflow, e.g. woffset == 32678, we know the previous bit changed is woffset - code_size(c), so until and exlcuding index woffset - code_size(c) write
        
		uint32_t nbytes = code_size(c);
		uint32_t pre_woffset =  woffset - code_size(c);
		uint32_t index = woffset - 32768; // woffset is the index NOT element/byte number
		woffset = 0;
		first_call = true;
        	while (code_size(c) > 0 && code_pop_bit(c, &bit)) { // if there were more bits to pop and woffset is ready to set a bit outside the block
                	printf("%u.", bit);
			if (bit == 1) {
                        	        bufbuf[index/8] = bufbuf[index/8] | (bit << (7 - index % 8));
                        	}else{
                                	bufbuf[index/8] = bufbuf[index/8] &  ~((one << (7 - index % 8)));    // 0001 0000 --> 1110 1111 &
                        	}
				if (index == 0) {
					break;
				}
			woffset++; // woffset will be set to the *top* of bufbuf
                	index--;
        	}
		woffset++; // ^^^ the break statment is blocking the last increment duh
		printf("bufbuf[127/8]: %u\n", bufbuf[127/8]);
		// woffset contains num of elements not the index
		uint32_t windex = 32767;
		while (pre_woffset != windex && code_pop_bit(c, &bit)) { // if there were more bits to pop and woffset is ready to set a bit outside the block
			if (bit == 1) {
                        	        wbuffer[windex/8] = wbuffer[windex/8] | (bit << (7 - windex % 8));
                        	}else{
                                	wbuffer[windex/8] = wbuffer[windex/8] &  ~((one << (7 - windex % 8)));    // 0001 0000 --> 1110 1111 &
                        	}
                	windex--;
        	}
		
		write_bytes(outfile, wbuffer, BLOCK);

        	if (nbytes != 0) {
			//printf("bufbuf[127/8]: %u\n", bufbuf[127/8]);
                	memcpy(wbuffer, bufbuf, 256);
			//printf("wbuffer[0]: %u\n", wbuffer[0]);
        	}
	
	}
}


void flush_codes(int outfile) {
        uint8_t one = 1;
	uint32_t flusher = 0;
	//bool needs_sth = false;
        //if (woffset + 1 % 8 != 0) { /// wbuffer will always be less than 32768
	if (woffset == 32767) {
		write_bytes(outfile, wbuffer, (woffset/8) + 1);
		return;
	}
	flusher = woffset + 1;
	while (flusher % 8 != 0) {
		wbuffer[flusher / 8]  = wbuffer[flusher / 8] & ~(one << (7 - (flusher % 8)));
		flusher++;
	}

	write_bytes(outfile, wbuffer, ((woffset - 1) / 8) + 1);
        return;	

	/*
	
        if (woffset + 1 % 8 != 0) { /// wbuffer will always be less than 32768 
	 	//needs_sth = true;
		wbuffer[woffset + 1 / 8]  = wbuffer[woffset + 1 / 8] & ~(one << (7 - ((woffset + 1) % 8)));
                woffset++;
	}else{
		write_bytes(outfile, wbuffer, (woffset/8) + 1);
		return;
	}

        write_bytes(outfile, wbuffer, ((woffset - 1)/8) + 1);
	*/
}

