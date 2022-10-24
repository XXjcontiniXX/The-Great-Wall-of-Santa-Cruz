#include <stdio.h>
#include "bubble.h"
#include <stdint.h>
#include "stats.h"
#include <stdlib.h>
#include "mtrand.h"
#include "shell.h"
#include "assert.h"
#include "heap.h"
#include "quick.h"
#include "set.h"
#include <unistd.h>
#include <inttypes.h>

#define OPTIONS "asbqhr:p:n:H"


int main(int argc, char **argv) {
        int opt = 0;
	static uint32_t p_elements = 100;
	static uint32_t n_elements = 100;
        mtrand_seed(13371453);
	Set tracker = set_empty();	
        while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
                switch(opt) {
                case 'a': // all sorts
			tracker = set_insert(tracker, 1);		
			break;
		case 's': // shell sort
			tracker = set_insert(tracker, 2);
			break;
		case 'b': // bubble sort
			tracker = set_insert(tracker, 3);
			break;
		case 'h': // heap sort
			tracker = set_insert(tracker, 4);
			break;
		case 'q': // quick sort
			tracker = set_insert(tracker, 5);
			break;
		case 'r': // seed	
			mtrand_seed(strtoull(optarg, NULL, 10));
			break;
		case 'n': // optarg number of elements
			if (strtoll(optarg, NULL, 10) > 2.5e8 || 0 > strtoll(optarg, NULL, 10)) { // greater than 0 and less or equal to 2.5e8
                                printf("./sorting.c: error: seed must be positive and less than 2.5e8\n");
                                exit(1);
                        }
			n_elements = strtoul(optarg, NULL, 10);
			break;
		case 'p': // prints out elements of the array
			tracker = set_insert(tracker, 6);
			p_elements = strtoul(optarg, NULL, 10);
			
			break;
		case 'H': // prints usage
			printf("SYNOPSIS\n   A collection of comparison-based sorting algorithms.\n\nUSAGE\n   ./sorting [-Hasbhq] [-n length] [-p elements] [-r seed]\n\nOPTIONS\n   -H              Display program help and usage.\n   -a              Enable all sorts.\n   -b              Enable Bubble Sort.\n   -h              Enable Heap Sort.\n   -q              Enable Quick Sort.\n   -s              Enable Shell Sort.\n   -n length       Specify number of array elements (default: 100).\n   -p elements     Specify number of elements to print (default: 100).\n   -r seed         Specify random seed (default: 13371453).\n");	
			exit(0);			
			break;
		default:
			printf("Invalid argument to the command line.\n");
			exit(1);
			break;	
		}
	}
	
	if (set_member(tracker, 1)) {
		tracker = set_insert(tracker, 2);
		tracker = set_insert(tracker, 3);
		tracker = set_insert(tracker, 4);
		tracker = set_insert(tracker, 5);
	
	} 
        uint32_t *array = (uint32_t *)malloc(n_elements * sizeof(uint32_t));
	static uint64_t mask = 0x000000003fffffff;
	for (uint32_t i = 0; i < n_elements; i++) {
		array[i] = mask & mtrand_rand64();
	}

		if (set_member(tracker, 2)) { // shell
			uint32_t *array_s = (uint32_t *)malloc(n_elements * sizeof(uint32_t));
			Stats shell;
			reset(&shell);
			for (uint32_t i = 0; i < n_elements; i++) {
				array_s[i] = array[i];
			}
			shell_sort(&shell, array_s, n_elements);
			printf("Shell Sort, %u elements, %lu moves, %lu compares\n", n_elements, shell.moves, shell.compares);
			int newline = -1;
			if (!set_member(tracker, 6)) {
                        	for (uint32_t i = 0; i < n_elements; i++) {
                                	newline += 1;
                                	if (newline % 5 == 0 && newline != 0) {
                                        	printf("\n");
                                	}
                                	printf("%13" PRIu32, array_s[i]);
                        }
                        	printf("\n");
			}else{
				for (uint32_t i = 0; i < p_elements; i++) {
                                newline += 1;
                                if (newline % 5 == 0 && newline != 0) {
                                        printf("\n");
                                }
                                printf("%13" PRIu32, array_s[i]);
                        }
                        printf("\n");
			}
			free(array_s);
			
		}
		if (set_member(tracker, 3)) { // bubble
		        uint32_t *array_b = (uint32_t *)malloc(n_elements * sizeof(uint32_t));
                        Stats bubble;
			reset(&bubble);
                        for (uint32_t i = 0; i < n_elements; i++) {
                                array_b[i] = array[i];
                        }
                        bubble_sort(&bubble, array_b, n_elements);
			printf("Bubble Sort, %u elements, %lu moves, %lu compares\n", n_elements, bubble.moves, bubble.compares);
			int newline = -1;
                        if (!set_member(tracker, 6)) {
                                for (uint32_t i = 0; i < n_elements; i++) {
                                        newline += 1;
                                        if (newline % 5 == 0 && newline != 0) {
                                                printf("\n");
                                        }
                                        printf("%13" PRIu32, array_b[i]);
                        }
                                printf("\n");
                        }else{
                                for (uint32_t i = 0; i < p_elements; i++) {
                                newline += 1;
                                if (newline % 5 == 0 && newline != 0) {
                                        printf("\n");
                                }
                                printf("%13" PRIu32, array_b[i]);
                        }
                        printf("\n");
                        }
                        free(array_b);
		
		} 
		if (set_member(tracker, 4)) { // heap
                        uint32_t *array_h = (uint32_t *)malloc(n_elements * sizeof(uint32_t));
                        Stats heap;
			reset(&heap);
                        for (uint32_t i = 0; i < n_elements; i++) {
                                array_h[i] = array[i];
                        }
                        heap_sort(&heap, array_h, n_elements);
			printf("Heap Sort, %u elements, %lu moves, %lu compares\n", n_elements, heap.moves, heap.compares);
                        int newline = -1;
                        if (!set_member(tracker, 6)) {
                                for (uint32_t i = 0; i < n_elements; i++) {
                                        newline += 1;
                                        if (newline % 5 == 0 && newline != 0) {
                                                printf("\n");
                                        }
                                        printf("%13" PRIu32, array_h[i]);
                        }
                                printf("\n");
                        }else{
                                for (uint32_t i = 0; i < p_elements; i++) {
                                newline += 1;
                                if (newline % 5 == 0 && newline != 0) {
                                        printf("\n");
                                }
                                printf("%13" PRIu32, array_h[i]);
                        }
                        printf("\n");
                        } 
                        free(array_h);
                        
		
		} 
		if (set_member(tracker, 5)) { // quick
			uint32_t *array_q = (uint32_t *)malloc(n_elements * sizeof(uint32_t));
                        Stats quick;
			reset(&quick);
                        for (uint32_t i = 0; i < n_elements; i++) {
                                array_q[i] = array[i];
                        }
                        quick_sort(&quick, array_q, n_elements);
                        printf("Quick Sort, %u elements, %lu moves, %lu compares\n", n_elements, quick.moves, quick.compares);
                        int newline = -1;
                        if (!set_member(tracker, 6)) {
                                for (uint32_t i = 0; i < n_elements; i++) {
                                        newline += 1;
                                        if (newline % 5 == 0 && newline != 0) {
                                                printf("\n");
                                        }
                                        printf("%13" PRIu32, array_q[i]);
                        }
                                printf("\n");
                        }else{
                                for (uint32_t i = 0; i < p_elements; i++) {
                                newline += 1;
                                if (newline % 5 == 0 && newline != 0) {
                                        printf("\n");
                                }
                                printf("%13" PRIu32, array_q[i]);
                        }
                        printf("\n");
                        }
                        free(array_q); 
		
		}

}

