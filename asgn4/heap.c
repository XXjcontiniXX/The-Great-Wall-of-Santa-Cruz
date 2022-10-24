#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "stats.h"
#include <assert.h>
#include "heap.h"

uint32_t l_child(uint32_t n) {
	return ((2 * n) + 1);
}

uint32_t r_child(uint32_t n) {
	return ((2 * n) + 2);
}

uint32_t parent(uint32_t n) {
	return ((n - 1) / 2);
}


void up_heap(Stats *stats, uint32_t *arr, uint32_t n) {
	while (n > 0 && cmp(stats, arr[n], arr[parent(n)]) == -1) { // if child less than parent child comes up
		swap(stats, &arr[n], &arr[parent(n)]); // smaller child becomes parent
		n = parent(n);  // checks to see if new parent is less than new parent
	}

}

void down_heap(Stats *stats, uint32_t *arr, uint32_t n_elements){
	uint32_t n = 0; // start at root
	uint32_t smaller;
	while (l_child(n) < n_elements) { // if there is a left child
		if (r_child(n) == n_elements) { // but no right child
						
			smaller = l_child(n); // left is only child
					      
		}else{ // else l_child and r_child return valid indices, lets compare. 
			if ( cmp(stats, arr[l_child(n)], arr[r_child(n)]) == -1)  { // left child bigger than right
				smaller = l_child(n); // right smaller.. else

			}else{
				smaller = r_child(n); // left smaller
			}
		}
		if (cmp(stats, arr[n], arr[smaller]) == -1) { // if parent is smaller than both children stop
			break;
			
		}
		swap(stats, &arr[n], &arr[smaller]); // if parent is bigger it needs to be put down one to the child and then the smallest child goes up
		n = smaller; // lets see if the ex-parent now child needs to go down anymore 
	}
	return;
}



uint32_t *build_heap (Stats *stats, uint32_t *arr, uint32_t n_elements) {
	uint32_t *new_heap = (uint32_t *)malloc(n_elements * sizeof(uint32_t));
	for (uint32_t n = 0; n < n_elements; n++) {
		new_heap[n]  = move(stats, arr[n]);
		up_heap(stats, new_heap, n);
	}
	return new_heap;
}

void heap_sort(Stats *stats, uint32_t *arr, uint32_t n_elements) {
	uint32_t *heap = build_heap(stats, arr, n_elements);
		//printf("build_heap runs\n");
		for (uint32_t n = 0; n < n_elements; n++) {
			arr[n] = move(stats, heap[0]);
			heap[0] = move(stats, heap[n_elements - n - 1]); // moves correct numbers into arr by putting heap[0] into index but the number at heap[0] gets deleted and replaced with the last number of the list. The list then gets downheaped and the next appopriate heap number is at heap[0].
			down_heap(stats, heap, n_elements - n);
		}
	free(heap);	
	return;	
}
