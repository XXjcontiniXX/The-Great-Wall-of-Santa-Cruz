#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "stats.h"
#include "bubble.h"


// struct will be initialized and inputted from in test-harness

void bubble_sort(Stats *stats, uint32_t *arr, uint32_t n_elements) {
	for (uint32_t i = 0; i < n_elements; i += 1) {  // from element one -> last element 
		int swapped = 0; // style is superior with pointer but saving arr for another idea
		int *ptr_swapped = &swapped;
		for (uint32_t j = n_elements - 1; j > i; j -= 1) { // iterate backwards from last element to the last previous unchecked element
			if (cmp(stats, arr[j], arr[j-1]) == -1) {   // if the front element is lesser than than the behind element swap
				swap(stats, &arr[j], &arr[j-1]); // first param needs to be address of struct, last params need to be adresses of swap elements
				*ptr_swapped = 1;  // if throughout whole check through array does at least one swap then array is not done
			
			} 
		
		
		}
	       	if (!swapped) {  // if true that swaps is false (not found)
			break; // actually finished / break main loop
		
		} 
		
	} 
	
}
