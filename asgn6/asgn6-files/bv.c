#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "bv.h"

struct BitVector {
 uint32_t length;
 uint64_t *vector;
 };


void bv_print(BitVector *bv) {
	for (uint32_t i = 0; i < bv_length(bv) * 64; i++) {
		if (i % 64 == 0) {
			printf("\n");
		}
		printf("%hu", bv_get_bit(bv, i));
	}
	printf("\n");
	return;
}


BitVector *bv_create(uint32_t length) {
	BitVector *bv = (BitVector *)malloc(sizeof(BitVector));
	bv->length = length;
	bv->vector = (uint64_t*)calloc(length, sizeof(uint64_t));
	if (bv->vector == NULL) {
		return NULL;
	}
	return bv;
}

void bv_delete(BitVector **bv) {
	if (*bv) {
		free((*bv)->vector);
		*bv = NULL;
	}
	return;
}

uint32_t bv_length(BitVector *bv) {
	return bv->length;
}

void bv_set_bit(BitVector *bv, uint32_t i) {
	uint64_t mask = 1UL << (i % 64); // inverse mask
	uint64_t chunk = ((i/64) % bv_length(bv)); // keeps big ass numbers inside of range 
	*((bv->vector) + (chunk)) = *((bv->vector) + (chunk)) | mask; // sets bit
}

void bv_clr_bit(BitVector *bv, uint32_t i) {
	uint64_t chunk = ((i/64) % bv_length(bv));
	uint64_t mask =  ~(1UL << (i % 64)); // mask
        *((bv->vector) + (chunk)) = *((bv->vector) + (chunk)) & mask; // unsets bit
}

uint8_t bv_get_bit(BitVector *bv, uint32_t i) {
	uint64_t chunk = ((i/64) % bv_length(bv));
	uint64_t mask = 1UL << (i % 64); // mask
	 if (mask == (bv->vector[chunk] & mask)) { // if mask stays the same then theres 
		return 1;
	 }else{
	 	return 0;
	 } 
}

