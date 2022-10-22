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
	uint32_t pivot_num = (arr[start] + arr[length/2] + arr[length-1]) / 3;
	uint32_t n_from_left = start; // increment eventually these number will converge at middle
        uint32_t n_from_right = length - 1; // decrement
	uint32_t modulo = 0;
	//printf("start = %u\n", arr[start]);
	//printf("mid = %u\n", arr[length/2]);
        //printf("length = %u\n", arr[length-1]);
	printf("pivot: %u\n", pivot_num);
	for (uint32_t i = start; i < length; i++) {
		printf("arr[i] :%u\n", arr[i]);
		if (arr[i] > pivot_num) {
			//printf("%u > %u\n",arr[i], pivot_num);
			swap(stats, &arr[i], &arr[n_from_right]);  //contintually number with elements cominfrom the right
			n_from_right -= 1; // index closer to middle
		}else if (arr[i] < pivot_num){
			//printf("%u < %u\n",arr[i], pivot_num);
			swap(stats, &arr[i], &arr[n_from_left]);
			 n_from_left += 1;
		}else{
			if (arr[(((length - start-1)/2) + start)] > arr[i]) {
				r_swap_sidw_w_less += 1;

			}else if (arr[(((length - start-1)/2) + start)] < arr[i]) {
				l_swap_side_w_less += 1;
				swap(stats, &arr[i], arr[start])
			
			}
			swap(stats, &arr[i], &arr[(((length - start-1)/2) + start)]);
			//printf("%u PIVOT NUM\n", (((length - start-1)/2) + start) );
			
		}
		       		// if not exist just move to arr[start] for small and arr[length-1] 
		}

//	printf("start = %u\n", start);
//	printf("length = %u\n", length);
	for (uint32_t i = start; i < length; i++) {
		printf("arr[%u] = %u\n", i, arr[i]);
	}
	//printf("start to n_from_left: %u\n", n_from_left - start);
	//printf("n_from_right to length: %u\n", length - n_from_right);
	assert(0);
	quick(stats, arr, start, n_from_left); 
	quick(stats, arr, n_from_right, length);
	
	}
			 
}

void quick_sort(Stats *stats, uint32_t *arr, uint32_t n_elements) {
	quick(stats, arr, 0, n_elements);
	return;

}
