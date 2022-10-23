#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "stats.h"
#include "shell.h"
#include <assert.h>


uint32_t SMALL = 8;

void quick(Stats *stats, uint32_t *arr, uint32_t start, uint32_t length){
	printf("NEW quick///////////////////");
	printf("partition: %u\n", length - start);
	if (length - start < SMALL) {
		printf("shelled\n");
		uint32_t *subarr = (uint32_t *)malloc((length - start) * sizeof(uint32_t));
		uint32_t j = 0;
		for (uint32_t i = start; i < length; i++) {
			subarr[j] = move(stats, arr[i]);
			j += 1;
		}
		shell_sort(stats, subarr, length - start);
		j = 0;
                for (uint32_t i = start; i < length; i++) {
                	arr[i] = move(stats, subarr[j]);
			j += 1;
                }
		free(subarr);
		return;	// find a way to sort a subarray whoop
	}else{
		uint32_t pivot_num = arr[length-1];
		uint32_t n_from_start = start; // increment eventually these number will converge at middle
       		uint32_t n_from_length = length - 1; // decrement
			for (uint32_t i = start; i < length; i++) {
				if (arr[i] > pivot_num) {
					swap(stats, &arr[i], &arr[n_from_length]);  //contintually number with elements cominfrom the right
					n_from_length -= 1; // index closer to middle
				}else if (arr[i] < pivot_num) {
					swap(stats, &arr[i], &arr[n_from_start]);
					n_from_start += 1;
				}
			}

	for (uint32_t i = start; i < length; i++) {
		printf("arr[%u] = %u\n", i, arr[i]);
	}
	
	}
        //quick(stats, arr, start, n_from_start);
	//quick(stats, arr, n_from_length, length);
}
			 


void quick_sort(Stats *stats, uint32_t *arr, uint32_t n_elements) {
	quick(stats, arr, 0, n_elements);
	return;

}
