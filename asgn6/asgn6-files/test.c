#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "str.h"
#include "bv.h"
#include "bf.h"
#include "node.h"


int main (void) {	
	//printf("oldspeak string = %s\n", buffer);
	BloomFilter *bf = bf_create(8);
	bf_insert(bf, "James");
	bf_insert(bf, "Jams");
	bf_print(bf);
	printf("James is in?: %d\n", bf_probe(bf, "James"));
	uint32_t n_keys;
        uint32_t n_hits;
        uint32_t n_misses;
        uint32_t n_bits_examined;
	bf_stats(bf, &n_keys, &n_hits, &n_misses, &n_bits_examined);

	printf("n_keys: %d\nn_hits = %d\nn_misses = %d\nn_bits_examined = %d\n", n_keys, n_hits, n_misses, n_bits_examined);
	//BitVector *bv = bv_create(8);
	//bv_set_bit(bv, 512);
	//bv_print(bv);
	//bf_insert(bf, "Hi");
	//char *oldspeak = "Charming";
	//char *newspeak = "Nice";
	//char *test = (char*)malloc((sizeof(char) * strlen_(newspeak)) + sizeof(char));
	//Node *n = node_create(oldspeak, newspeak);
	//strcpy_(test, newspeak);
	//int check = strcmp_(test, newspeak);
	//printf("is test null terminated?: %d\n", check);
	//node_print(n);
	return 0;
}
