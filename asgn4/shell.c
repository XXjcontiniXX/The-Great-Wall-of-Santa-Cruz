#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "stats.h"
#include "bubble.h"
#include <assert.h>

uint32_t next_gap (uint32_t length) {
	
	int32_t n = length; // set a dynamic n
	if (n == 1) {
		return 0;
	}
	if (n <= 2) {
		n = 1; // if n 2 or less n = 1 
	}else{
		n = (5 * n) / 11; // if n greater than 2 n = n/2 - some
	}

	return n;   	
}





void shell_sort(Stats *stats, uint32_t *arr, uint32_t n_elements) {
	for (uint32_t gap = next_gap(n_elements); gap > 0; gap = next_gap(gap)) {
		if ( gap == 0 ) {
			break;
		}
		for (uint32_t i = gap; i < n_elements; i += 1) {
			uint32_t j = i;
			uint32_t temp = move(stats, arr[i]);
			while (j >= gap && cmp(stats, temp, arr[j-gap]) == -1) { // (j >= gap && temp < arr[j-gap]) /// if the elements on opposite sides of the gap are in the wrong spots keep switching by one gap backwards until its on the gap that it should stay on
				arr[j] = move(stats, arr[j - gap]); // move it back one gap if its needs to be 
				j -= gap;  // move back one gap back
			
			}
			arr[j] = move(stats, temp); 
		
		}		
		
	} 
	return;
}
