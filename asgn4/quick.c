#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "stats.h"
#include "shell.h"
#include <assert.h>


uint32_t SMALL = 8;

void quick(Stats *stats, uint32_t *arr, uint32_t left, uint32_t right);

void quick_sort(Stats *stats, uint32_t *arr, uint32_t n_elements) {
        quick(stats, arr, 0, n_elements);
	return;
}


void quick(Stats *stats, uint32_t *arr, uint32_t start, uint32_t length){	
	if (length - start < SMALL) {
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
		return;	
	}else{
		uint32_t left = 0;
		uint32_t right = 0;
		uint32_t pivot = arr[length - 1];
		uint32_t *copy = (uint32_t *)calloc(length, sizeof(uint32_t));
	        for (uint32_t i = start; i < length; i++) {
			if (arr[i] < pivot) {
				copy[start + left] = move(stats, arr[i]);
				left += 1;	
			} else if (arr[i] > pivot) {
				copy[length - 1 - right] = move(stats, arr[i]);
				right += 1;
			}

		}
		
		for (uint32_t i = start; i < length; i++) {
			if (copy[i] != 0) {
				arr[i] = move(stats, copy[i]);
			}else{
				arr[i] = move(stats, pivot);
				
			}
		}
		free(copy);
		if (left == 0){
			quick(stats, arr, start, start); 
		}else{
			quick(stats, arr, start, start + left - 1); 
		}
		if (right == 0) {
			
		}	
			quick(stats, arr, length - right, length);
		} 

	}
	

			 



