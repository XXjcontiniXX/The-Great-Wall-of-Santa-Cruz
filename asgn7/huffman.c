#include "huffman.h"
#include "node.h"
#include "code.h"
#include "pq.h"
#include "defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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
		node_print(parent);
		node_print(parent->left);
		node_print(parent->right);
		enqueue(q, parent);	
	}

	Node *root = NULL;
	dequeue(q, &root);

	return root;
}

void build_codes(Node *root, Code table[static ALPHABET]) {
	Code c = code_init();
	uint8_t garbit;
	if (root != NULL) {
		if (root->left == NULL && root->right == NULL) {
			table[root->symbol] = c;
			printf("root->symbol %u\n", root->symbol);
			code_print(&c);
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

void dump_tree(int outfile, Node *root);

Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]);

void delete_tree(Node **root);
