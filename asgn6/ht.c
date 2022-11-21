#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "str.h"
#include "ll.h"
#include "ht.h"
#include "city.h"
#include <stdbool.h>

struct HashTable {
	uint64_t salt;
	uint32_t size;
	uint32_t n_keys;
	uint32_t n_hits;
	uint32_t n_misses;
	uint32_t n_examined;
	bool mtf ;
	LinkedList ** lists;
};



HashTable *ht_create(uint32_t size, bool mtf) {
	HashTable *ht = ( HashTable *) malloc ( sizeof ( HashTable ));
	if (ht != NULL ) {
		ht->mtf = mtf ;
		ht->salt = 0x9846e4f157fe8840;
		ht->n_hits = ht->n_misses = ht->n_examined = 0;
		ht->n_keys = 0;
		ht->size = size;
		ht->lists = (LinkedList **)calloc(size, sizeof(LinkedList *));
		if (!ht->lists) {
			free(ht);
			ht = NULL;
		}
	}
	return ht;
}




void ht_delete(HashTable **ht) {
	for (uint32_t i = 0; i < ht_size(*ht); i++) {
		if (*(((*ht)->lists) + i) != NULL)  {
			ll_delete(&(*(((*ht)->lists) + i)));
		}
	}
	*ht = NULL;
}

uint32_t ht_size(HashTable *ht) {
	return ht->size;
}

Node *ht_lookup(HashTable *ht, char *oldspeak) {
	uint64_t index = hash(ht->salt, oldspeak) % ht_size(ht); // index always in table - maybe empty tho
	uint32_t pre_seeks = 0;
	uint32_t pre_links = 0;
	uint32_t post_seeks = 0;
	uint32_t post_links = 0;
	LinkedList *ll = *((ht->lists) + index); // TODO check if appropiate to count node exams if no node was found?
	if (ll != NULL) { // is ll we looking at NULL
		ll_stats(&pre_seeks, &pre_links); // we can get the ll's stats if its not NULL
		Node *ans = ll_lookup(ll, oldspeak);
		if (ans == NULL) {
			ht->n_misses += 1;
		}else{
			ht->n_hits += 1;
		}
		ll_stats(&post_seeks, &post_links); // after lookup stats
		ht->n_examined += post_links - pre_links; // change stats
		return ans; // return the node
	} // ok then do nothing	
	ht->n_misses += 1;
	return NULL;
}

void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) { 
	uint64_t index = hash(ht->salt, oldspeak) % ht_size(ht);
	uint32_t test = 0;
	if (*((ht->lists) + index) == NULL) { // if spot is NULL make an ll
		*((ht->lists) + index) = ll_create(ht->mtf);
		ll_insert(*((ht->lists) + index), oldspeak, newspeak);
		ht->n_keys += 1;
	}else {
		test = ll_length(*((ht->lists) + index));
		ll_insert(*((ht->lists) + index), oldspeak, newspeak);
		if (test != ll_length(*((ht->lists) + index))) {
			ht->n_keys += 1;
		}
	}
}


uint32_t ht_count(HashTable *ht) {
	uint32_t count = 0;
	for (uint32_t i = 0; i < ht_size(ht); i++) {
                if (*((ht->lists) + i) != NULL)  {
                        count += 1;
                }
        }
	return count;
}

void ht_print(HashTable *ht) {
	for (uint32_t i = 0; i < ht_size(ht); i++) {
		if (*((ht->lists) + i) != NULL) {
			ll_print(*((ht->lists) + i));
		}
	}
}

void ht_stats(HashTable *ht, uint32_t *nk, uint32_t *nh, uint32_t *nm, uint32_t *ne) {
	*nk = ht->n_keys;
        *nh = ht->n_hits;
        *nm = ht->n_misses;
        *ne = ht->n_examined;
}
