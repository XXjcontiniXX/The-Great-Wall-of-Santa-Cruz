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



int main(void) {
	//Stats bubble;
	Stats quick;
        //uint32_t bubble_number[] = {10,3,1,4,7,4,3,2,8,10};
	uint32_t quick_number[]	= {1, 3, 5, 4, 6, 13, 10, 9, 8, 15, 17, 8,1,4,6,6,7,8,5,4,3,2,5,6,8,9,0,10,4,7};
	//bubble_sort(&bubble, bubble_number, 10); // remember, n_elements is the number of element so if theres 10 elements this number is 10 **not** the max index number (9)
	quick_sort(&quick, quick_number, 30);
	
	
	for (int i = 0; i < 30; i+=1) { 
		printf("%u\n", quick_number[i]);
	}
	printf("%lu\n", quick.moves); 

}
