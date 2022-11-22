#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bv.h"
#include "bf.h"
#include "node.h"
#include "ll.h"
#include "ht.h"
#include "parser.h"
#include "messages.h"
#include <unistd.h>
#define OPTIONS "t:f:smh"

int main(int argc, char **argv) {
  int opt = 0;
  uint8_t v = 0;
  char help[] =
      "Usage: ./banhammer [options]  \n./banhammer will read in words from stdin, identify any badspeak or old speak and output an  \nappropriate punishment message. The badspeak and oldspeak (with the newspeak translation)\n  that caused the punishment will be printed after the message. If statistics are enabled\n  punishment messages are suppressed and only statistics will be printed.\n    -t <ht_size>: Hash table size set to <ht_size>. (default: 10000)    \n-f <bf_size>: Bloom filter size set to <bf_size>. (default 2^19)\n    -s          : Enables the printing of statistics.    \n-m          : Enables move-to-front rule.    \n-h          : Display program synopsis and usage.\n";
	uint32_t bf_size = 524288; 
	uint32_t ht_size = 10000;
	bool mtf = false;
	FILE *badspeak;
	FILE *dict;

	while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
		switch (opt) {
			case 't':
				ht_size = strtoul(optarg, NULL, 10);
      				break;
    			case 'f':
				bf_size = strtoul(optarg, NULL, 10);
      				break;
    			case 's':
				v = v | (1 << 0); // if v == 1 print stats
      				break;
    			case 'm':
				mtf = true;
      				break;
    			case 'h':
				v = v | (1 << 1); // if v > 1 only print help
      				break;
    			default:
      				v = v | (1 << 2);
      				break;
   		}
  	}
	
	if (v > 2) {
		fprintf(stderr, "%s", help);
		return 1;
	
	}
	if (v > 1) {
		fprintf(stderr, "%s", help);
		return 0;
	}

	BloomFilter *bf = bf_create(bf_size);
	HashTable *ht = ht_create(ht_size, mtf);
	

	badspeak = fopen("badspeak.txt", "r");
	dict = fopen("newspeak.txt", "r");
	
	char *buffer2 = (char*)malloc(sizeof(char)*1001);
	char *buffer1 = (char*)malloc(sizeof(char)*1001);
	Parser *p_badspeak = parser_create(badspeak);
	while (next_word(p_badspeak, buffer1)) {
		bf_insert(bf, buffer1);
		ht_insert(ht, buffer1, NULL); // TODO gotta check if this is functioning
	}
	

	Parser *p_dict = parser_create(dict);
	while (next_word(p_dict, buffer1)){
		next_word(p_dict, buffer2);
		bf_insert(bf, buffer1);
		ht_insert(ht, buffer1, buffer2);
	}
	
	LinkedList *ll_badspeak = ll_create(false);
	LinkedList *ll_oldspeak = ll_create(false);


	Parser *p = parser_create(stdin);
	while (next_word(p, buffer1)) { // buffer 1 is being reused // for every word in stdin
		if (bf_probe(bf, buffer1)) {
			Node *test_node = ht_lookup(ht, buffer1);
			if (test_node == NULL) { //false positive TODO n_misses i think its already counted
				continue;
			} else if (test_node->newspeak == NULL) { // BADSPEAK thoughtcrime
				ll_insert(ll_badspeak, test_node->oldspeak, NULL); // records the oldspeak
			}else{ // rightspeak there is a translation 
				ll_insert(ll_oldspeak, test_node->oldspeak, test_node->newspeak); // list
			}
		}
	}
	
	if (ll_length(ll_badspeak) >= 1 && ll_length(ll_oldspeak) >= 1) {
		printf("%s", mixspeak_message);
		ll_print(ll_badspeak);
		ll_print(ll_oldspeak);
	}else if(ll_length(ll_badspeak) >= 1) {
		printf("%s", badspeak_message);
		ll_print(ll_badspeak);
	
	}else if (ll_length(ll_oldspeak) >= 1){
		printf("%s", goodspeak_message);
		ll_print(ll_oldspeak);
	}
	return 0;
}


