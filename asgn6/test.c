#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "str.h"
#include "bv.h"
#include "bf.h"
#include "node.h"
#include "ll.h"
#include "ht.h"

int main (void) {	
	//printf("oldspeak string = %s\n", buffer);
	/*BloomFilter *bf = bf_create(8);
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
	*/
	/*uint32_t seeks;
	uint32_t links;
	LinkedList *ll = ll_create(false);
	ll_insert(ll, "charming", "nice");
	ll_insert(ll, "bisexual", "unstraight");
	ll_insert(ll, "gay", "unstraight");
	
	//(void)ll_lookup(ll, "gay");
	(void)ll_lookup(ll, "bisexual");
	//(void)ll_lookup(ll, "charming");
	//node_print(ll_lookup(ll, "charming"));
	//ll_print(ll);
	//ll_delete(&ll);
	ll_print(ll);
	ll_stats(&seeks, &links);
	printf("Seeks: %u\nLinks: %u\n", seeks, links);*/
	HashTable *ht = ht_create(8, true);
	ht_insert(ht, "charming", "nice");
	ht_insert(ht, "guy", "human");
	ht_insert(ht, "funny", "personality");
	ht_insert(ht, "charming", "nice");
	ht_print(ht);
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
