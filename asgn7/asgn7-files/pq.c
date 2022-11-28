#include "node.h"
#include "pq.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

struct PriorityQueue {
	uint32_t capacity;
	uint64_t *freqs;
	uint8_t *symbols;
	uint32_t offset;

};

void swap_64(uint64_t *x, uint64_t *y) {
	uint32_t t = *x;
	*x = *y;
	*y = t;
}

void swap_8(uint8_t *x, uint8_t *y) {
        uint8_t t = *x;
        *x = *y;
        *y = t;
}


PriorityQueue *pq_create(uint32_t capacity) {
	PriorityQueue *q = (PriorityQueue*)malloc(sizeof(PriorityQueue));	
	if (q) {
		q->capacity = capacity;
		uint64_t *freqs = (uint64_t*)calloc(capacity, sizeof(uint64_t));
		uint8_t *symbols = (uint8_t*)calloc(capacity, sizeof(uint8_t));
		q->freqs = freqs;
		q->symbols = symbols;
		q->offset = 0;
	}
	return q;
}

void pq_delete(PriorityQueue **q) {
	if ((*q)) {
		free((*q)->freqs);
		free((*q)->symbols);
		free(*q);
		*q = NULL;
	}
	return;
}

bool pq_empty(PriorityQueue *q) {
	if (pq_size(q) == 0) {
		return true;
	}else{
		return false;
	}
}

bool pq_full(PriorityQueue *q) {
	if (pq_size(q) == q->capacity) {
		return true;
	}else{
		return false;
	}
}

uint32_t pq_size(PriorityQueue *q) {
	uint32_t j = 0;
	for (uint32_t i = 0; i < q->capacity; i++) {
		if ((q->freqs)[i] > 0) {
			j++;
		}
	}
	return j;
}


bool enqueue(PriorityQueue *q, Node *n) {
	if (pq_full(q)) {
		return false;
	}else{ 
		if (pq_empty(q)) {
			q->freqs[0] = n->frequency; // insert to free spots
                	(q->symbols)[0] = n->symbol;
			q->offset = 0;
		}else{
			for (uint32_t i = 0; i < q->capacity; i++) {
				if (q->freqs[i] == 0) {
					q->freqs[i] = n->frequency;
					(q->symbols)[i] = n->symbol;
					q->offset = i;
					break;
				}
			}
		}
		// insert time
		uint32_t index = q->offset; // start at end of array
		while (index > 0 && q->freqs[index] < q->freqs[index - 1]) {
			swap_64(&q->freqs[index], &q->freqs[index - 1]);
			swap_8(&(q->symbols)[index], &(q->symbols)[index - 1]);
			index -= 1;
		}	
	}
	return true;

}			


bool dequeue(PriorityQueue *q, Node **n) {
	if (pq_empty(q)) {
		return false;
	}else{
		uint64_t frequency = q->freqs[0];
		uint8_t symbol = q->symbols[0];
		q->symbols[0] = 0;
		q->freqs[0] = 0;
		(*n) = node_create(symbol, frequency);
			                        
		for (uint32_t i = 0; i < q->offset && q->freqs[i + 1] != 0; i++) {
			q->freqs[i] = q->freqs[i + 1];
			q->freqs[i + 1] = 0;
			q->symbols[i] = q->symbols[i + 1];
			q->symbols[i + 1] = 0;
		}
		return true;
	}


}

void pq_print(PriorityQueue *q) {
	for (uint32_t i = 0; i < q->capacity; i++) {
		if ((q->freqs)[i] != 0) {
			Node *n = node_create((q->symbols)[i], (q->freqs)[i]);
			node_print(n);
		}
	}
}

