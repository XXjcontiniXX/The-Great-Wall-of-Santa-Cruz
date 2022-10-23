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

#define OPTIONS "asbqhr:pn:H"


int main(int argc, char **argv) {
        int opt = 0;
	static uint32_t p_elements;
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
			p_elements = strtoul(optarg, NULL, 10);
			tracker = set_insert(tracker, 6);
			break;
		case 'H': // prints usage
			printf("SYNOPSIS\n   A collection of comparison-based sorting algorithms.\n\nUSAGE\n   ./sorting [-Hasbhq] [-n length] [-p elements] [-r seed]\n\nOPTIONS\n   -H              Display program help and usage.\n   -a              Enable all sorts.\n   -b              Enable Bubble Sort.\n   -h              Enable Heap Sort.\n   -q              Enable Quick Sort.\n   -s              Enable Shell Sort.\n   -n length       Specify number of array elements (default: 100).\n   -p elements     Specify number of elements to print (default: 100).\n   -r seed         Specify random seed (default: 13371453).\n");	
			exit(0);			
			break;
			
		}
	}
	if (set_member(tracker, 1)) {
		printf("run all");
	
	}else{
                uint32_t *array = (uint32_t *)malloc(n_elements * sizeof(uint32_t));
		static uint64_t mask = 0x000000003fffffff;
		for (uint32_t i = 0; i < n_elements; i++) {
			array[i] = mask & mtrand_rand64();
		}

		if (set_member(tracker, 2)) { // bubble
			uint32_t *array_s = (uint32_t *)malloc(n_elements * sizeof(uint32_t));
			Stats shell;
			for (uint32_t i = 0; i < n_elements; i++) {
				array_s[i] = array[i];
			}
			shell_sort(&shell, array_s, n_elements);
			for (uint32_t i = 0; i < n_elements; i++) {
				printf("%u\n", array_s[i]);
			}
			free(array_s);
			printf("shell\n");
			
		}
		if (set_member(tracker, 3)) { // shell
		        uint32_t *array_b = (uint32_t *)malloc(n_elements * sizeof(uint32_t));
                        Stats bubble;
                        for (uint32_t i = 0; i < n_elements; i++) {
                                array_b[i] = array[i];
                        }
                        bubble_sort(&bubble, array_b, n_elements);
                        for (uint32_t i = 0; i < n_elements; i++) {
                                printf("%u\n", array_b[i]);
                        }
                        free(array_b);
                        printf("bubble\n");
		
		} 
		if (set_member(tracker, 4)) { // heap
                        uint32_t *array_h = (uint32_t *)malloc(n_elements * sizeof(uint32_t));
                        Stats heap;
                        for (uint32_t i = 0; i < n_elements; i++) {
                                array_h[i] = array[i];
                        }
                        heap_sort(&heap, array_h, n_elements);
                        for (uint32_t i = 0; i < n_elements; i++) {
                                printf("%u\n", array_h[i]);
                        }
                        free(array_h);
                        printf("heap\n");
		
		} 
		if (set_member(tracker, 5)) { // quick
                        uint32_t *array_q = (uint32_t *)malloc(n_elements * sizeof(uint32_t));
                        Stats quick;
                        for (uint32_t i = 0; i < n_elements; i++) {
                                array_q[i] = array[i];
                        }
                        quick_sort(&quick, array_q, n_elements);
                        for (uint32_t i = 0; i < n_elements; i++) {
                                printf("%u\n", array_q[i]);
                        }
                        free(array_q);
			printf("quick\n");
		
		}
		if (set_member(tracker, 6)) { // prints elements
			printf("elements\n");

		}


		}
}


