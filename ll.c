#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ll.h"
#include <stdbool.h>


uint64_t seeks; // Number of seeks performed.
uint64_t links;

struct LinkedList {
	uint32_t length;
	Node * head ; // Head sentinel node .
	Node * tail ; // Tail sentinel node .
	bool mtf ;
 };


int strcmp_(const char *s1, const char *s2) {
        uint32_t i = 0;
        if (s1 == NULL && s2 == NULL) {
                return 1;
        }
        for (; s1[i] != '\0'; i++) {
                uint8_t sum =  (unsigned char)(s1[i]) - (unsigned char)(s2[i]);
                if (sum != 0) {
                        return 0;
                }

        }
        return !((unsigned char)(s1[i]) - (unsigned char)(s2[i])); // only returns 1 when same ending                                                                      // else some random number
}

LinkedList *ll_create(bool mtf) {
	LinkedList *ll = (LinkedList *)malloc(sizeof(LinkedList));	
	if (ll) {
		ll->mtf = mtf;
		ll->length = 0;
		ll->head = node_create(NULL, NULL);
		ll->tail = node_create(NULL, NULL);
		Node *node_head = ll->head;
		Node *node_tail = ll->tail;
		node_head->next = node_tail;
		node_tail->prev = node_head;
	}
	return ll;
}

void ll_delete(LinkedList **ll) {
	Node *next = (*ll)->head; // next
	while (next != (*ll)->tail) { 
		Node *curr = NULL;
		curr = next; // curr becomes one more than last iterations
		next = curr->next; // next becomes one more than this iteration
		node_delete(&curr); // delete one more than last iteration
	}
	node_delete(&((*ll)->tail)); // since the tail->next == null we have to delete it manually		
	*ll = NULL;
	

}

Node *ll_lookup(LinkedList *ll, char *oldspeak) {
	seeks += 1;
	for (Node *curr = (ll->head)->next; curr != ll->tail; curr = curr->next) {
		links += 1;
		if (strcmp_(curr->oldspeak, oldspeak) == 1) {
			if (ll->mtf && ((ll->head))->next != curr) {
				Node *second = ((ll->head)->next); // the original first node
				Node *before_curr = curr->prev;
				Node *after_curr = curr->next;
				// fix head's pointer            
				(ll->head)->next = curr;
				// fix original first node's pointers
				second->prev = curr;
				// fix the node before and after curr's pointers
				before_curr->next = after_curr; // before nexts to after
				after_curr->prev = before_curr; // after prevs to before
				// fix curr pointers
				curr->next = second;
				curr->prev = ll->head;
			}
			return curr;
		}
	}
	return NULL;
}

uint32_t ll_length(LinkedList *ll) {
	return ll->length;
	
}

void ll_insert(LinkedList *ll, char *oldspeak, char *newspeak) {
	if (ll_lookup(ll, oldspeak) == NULL) {
		Node *node = node_create(oldspeak, newspeak); // new node
		Node *head = ll->head; // head node
		Node *first_node = head->next; // first real node
		node->next = first_node; // new node next is first_node
		node->prev = head; // new node prev is head
		first_node->prev = node; // first_node prev now is new node
		head->next = node; // head points to new node
		ll->length = ll->length + 1; // length increases
	}
	
}

void ll_print(LinkedList *ll) {
	Node *head = ll->head;
	for (Node *curr = head->next; curr != ll->tail; curr = curr->next) {
		node_print(curr);
		
	}
	return;
 
}

void ll_stats(uint32_t *n_seeks, uint32_t *n_links) {
	*n_seeks = seeks;
	*n_links = links;
	return;	
}
