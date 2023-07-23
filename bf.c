#include <stdio.h>
#include <stdlib.h>
#include "bf.h"
#include "bv.h"
#include <stdint.h>
#include "city.h"
#define N_HASHES 5

static uint64_t default_salts [] = {
 0x5adf08ae86d36f21 ,
 0x419d292ea2ffd49e ,
 0x50d8bb08de3818df ,
 0x272347aea4045dd5 ,
 0x7c8e16f768811a21
 };

struct BloomFilter {
	uint64_t salts[N_HASHES]; // Salts for five hash functions
	uint32_t n_keys;
	uint32_t n_hits;
	uint32_t n_misses;
	uint32_t n_bits_examined;
	BitVector *filter;
};

BloomFilter *bf_create(uint32_t size) {
	BloomFilter *bf = (BloomFilter *)malloc(sizeof(BloomFilter));
	if (bf) {
		bf->n_keys = bf->n_hits = 0; // n_keys and n_bits is 0
		bf->n_misses = bf->n_bits_examined = 0; // setting to 0
		for (int i = 0; i < N_HASHES; i++) {
			bf->salts[i] = default_salts[i]; // 
		}
		bf->filter = bv_create(size); // the filter is actual a bv (bit vector)
		if (bf->filter == NULL) {
			free(bf);
			bf = NULL;
		}
	}
	return bf;
}

void bf_delete(BloomFilter **bf) {
	if (*bf) {
		bv_delete(&((*bf)->filter)); // must implement bv_delete before usage
		free((*bf)->salts);
		free(*bf);
		*bf = NULL;
	}
	return;
}

uint32_t bf_size(BloomFilter *bf) {
	return bv_length(bf->filter); // how many uin64s it can hold times 64 for the bits

}

void bf_insert(BloomFilter *bf, char *oldspeak) {
	uint64_t index;
	bf->n_keys = bf->n_keys + 1;
	for (int j = 0; j < 5; j++) {
        	index = hash(bf->salts[j], oldspeak); // what bv index to set (subject to change)
		bv_set_bit(bf->filter, index); // sets that index
        }
	return;
}


bool bf_probe(BloomFilter *bf, char *oldspeak){
	uint64_t index;
	for (int i = 0; i < 5; i++) {
		bf->n_bits_examined = bf->n_bits_examined + 1;
		index = hash(bf->salts[i], oldspeak);
		if (bv_get_bit(bf->filter, index) == 0) { // if 1 hash function def didn't hash str bye
			bf->n_misses += 1;
			return false;
		}	
	}
	bf->n_hits = bf->n_hits + 1;
	return true;
}

uint32_t bf_count(BloomFilter *bf) {
	uint32_t length = bv_length(bf->filter);
	uint32_t j = 0;
	for (uint32_t i = 0; i < length; i++) {
		if (bv_get_bit(bf->filter, i)) {
			j++;
		}
	
	}
	return j;
}

void bf_print(BloomFilter *bf) {
	bv_print(bf->filter);
	return;
}

void bf_stats(BloomFilter *bf, uint32_t *nk, uint32_t *nh, uint32_t *nm, uint32_t *ne) {
	*nk = bf->n_keys;
	*nh = bf->n_hits;
	*nm = bf->n_misses;
	*ne = bf->n_bits_examined;
	return;
}
