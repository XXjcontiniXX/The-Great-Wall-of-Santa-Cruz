#include "huffman.h"
#include "node.h"
#include "code.h"
#include "pq.h"
#include "defines.h"
#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static bool began = false;
static bool started = false;
static Code c;


static Code L;
static Code L_copy;

static Code I;
static Code I_copy;




Node *build_tree(uint64_t hist[static ALPHABET]) {
	PriorityQueue *q = pq_create(ALPHABET);
	for (uint32_t i = 0; i < ALPHABET; i++) {
		if (hist[i] > 0) {
			enqueue(q, node_create(i, hist[i]));
		}
	}
	while (pq_size(q) > 1) {
		Node *left = NULL;
		Node *right = NULL;
		Node *parent = NULL;
		dequeue(q, &left);
		dequeue(q, &right);
		parent = node_join(left, right);
		enqueue(q, parent);	
	}

	Node *root = NULL;
	dequeue(q, &root);

	return root;
}


void build_codes(Node *root, Code table[static ALPHABET]) {
	uint8_t garbit;
	
	if (started == false) {
		Code d = code_init();
		c = d;
		started = true;
	}

	if (root != NULL) {
		if (root->left == NULL && root->right == NULL) {
			table[root->symbol] = c;
		}else{
			code_push_bit(&c, 0);
			build_codes(root->left, table);
			code_pop_bit(&c, &garbit);
			
			code_push_bit(&c, 1);
			build_codes(root->right, table);
			code_pop_bit(&c, &garbit);
		}
	}
	return;
}

void dump_tree(int outfile, Node *root) {
	uint8_t bit;
	uint8_t byte = 1;
	if (began == false) {
		for (uint8_t i = 0; i < 8; i++) {
        		bit = (bool)('L' & (byte << (7 - i % 8)));
			code_push_bit(&L, bit);
		}
			
		for (uint8_t i = 0; i < 8; i++) {
                	bit = (bool)('I' & (byte << (7 - i % 8)));
                	code_push_bit(&I, bit);
        	}
		began = true;
	}
	//code_print(&L);
	
	if (root != NULL) {
		dump_tree(outfile, root->left);
		dump_tree(outfile, root->right);

		if (root->left == NULL && root->right == NULL) {
			Code s = code_init();
			L_copy = L;
			write_code(outfile, &L);  // write binary char to file
			L = L_copy;
			
			for (uint8_t i = 0; i < 8; i++) {
                		bit = (bool)(root->symbol & (byte << (7 - i % 8)));
                		code_push_bit(&s, bit);
        		}
			write_code(outfile, &s); // write symbol
		}else{
			I_copy = I;
			write_code(outfile, &I);
			I = I_copy;
		}
	}
}

Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]);

void delete_tree(Node **root);
