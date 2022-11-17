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
		ll_delete(((*ht)->lists) + i);
	}
	*ht = NULL;
	
}

uint32_t ht_size(HashTable *ht) {
	return ht->size;
}

Node *ht_lookup(HashTable *ht, char *oldspeak) {
	uint64_t index = hash(ht->salt, oldspeak) % ht_size(ht); // index always in table - maybe empty tho
	return ll_lookup(*((ht->lists) + index), oldspeak) == NULL ? NULL : ll_lookup(*((ht->lists) + index), oldspeak);
	
}

void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) { 
	uint64_t index = hash(ht->salt, oldspeak) % ht_size(ht);
	if (*((ht->lists) + index) == NULL) { // if spot is NULL make an ll
		*((ht->lists) + index) = ll_create(ht->mtf);
		ll_insert(*((ht->lists) + index), oldspeak, newspeak);
	}else {
		ll_insert(*((ht->lists) + index), oldspeak, newspeak);
	}
}

/*
uint32_t ht_count(HashTable *ht) {
	// returns *NON* NULL lls in hash table
	for (uint32_t i = 0; i < ht_size(ht); i++) {
		if () {
		
		}
	}
}
*/
void ht_print(HashTable *ht) {
	for (uint32_t i = 0; i < ht_size(ht); i++) {
		if (*((ht->lists) + i) != NULL) {
			ll_print(*((ht->lists) + i));
		}
	}
}

void ht_stats(HashTable *ht, uint32_t *nk, uint32_t *nh, uint32_t *nm, uint32_t *ne);
