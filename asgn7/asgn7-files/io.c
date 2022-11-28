#include "code.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

uint64_t bytes_read = 0;
uint64_t bytes_written = 0;

int read_bytes(int infile, uint8_t *buf, int nbytes) {
	int i = 0;
	int j;
	while ((j = read(infile, buf + i, nbytes - i)) > 0) {
		i += j;
	}
	bytes_read += i;
	return i;	
}

int write_bytes(int outfile, uint8_t *buf, int nbytes) {
	int i = 0;
	int j;
	while ((j = write(outfile, buf + i, nbytes - i)) > 0) {
                i += j;
        }
        bytes_written += i;
        return i;
}

bool read_bit(int infile, uint8_t *bit);

void write_code(int outfile, Code *c);

void flush_codes(int outfile);
