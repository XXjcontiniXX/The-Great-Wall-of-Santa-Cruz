#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "str.h"
#include "node.h"
/*
struct Node {
    char *oldspeak;
    char *newspeak;
    Node *next;
    Node *prev;
};
*/

Node *node_create(char *oldspeak, char *newspeak) {
	Node *node = (Node *)malloc(sizeof(Node));
	if (node) {
		if (oldspeak != NULL) {
			char *old_speak = (char*)malloc((sizeof(char) * strlen_(oldspeak)) + sizeof(char));
        		char *new_speak = (char*)malloc((sizeof(char) * strlen_(newspeak)) + sizeof(char));
			strcpy_(old_speak, oldspeak);
			strcpy_(new_speak, newspeak);
			node->oldspeak = old_speak;
			node->newspeak = new_speak;
		}else{
			char *sentinal1 = NULL;
			char *sentinal2 = NULL;
			node->oldspeak = sentinal1;
			node->newspeak = sentinal2;
		}
		node->prev = NULL;
	        node->next = NULL;	
	}
	return node;
}

void node_delete(Node **n) {
	if (*n) {
		free((*n)->oldspeak);	
		free((*n)->newspeak);
		free(*n);
		*n = NULL;
	}
	return;	
}

void node_print(Node *n) {
	if (n) {
		if (n->newspeak == NULL) {
			printf("%s\n", n->oldspeak);
		}else{
			printf ("%s -> %s\n", n->oldspeak, n->newspeak);
		}
	}
	return;
}
