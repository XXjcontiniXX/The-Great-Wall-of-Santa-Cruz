#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "node.h"
#include "pq.h"
#include "code.h"
#include "io.h"


int main(void) {
	// NODE TESTING
	/*
	Node *a = node_create('a', 5);
	Node *b = node_create('b', 20);
	Node *c = node_create('c', 40);
	Node *d = node_create('d', 60);
	Node *e = node_create('e', 100);
	Node *z;
	node_print(n);
	printf("\n");
	node_print(m);
	printf("n is %d greater than m\n", node_cmp(m, m));
	*/
	// END NODE TESTING
	// PQ TESTING
	/*
	PriorityQueue *q = pq_create(7);
	enqueue(q, e);
	enqueue(q, a);
       	enqueue(q, b);
	enqueue(q, c);
	enqueue(q, d);
	enqueue(q, b);
	enqueue(q, b);
	pq_print(q);
	dequeue(q, &z);
	printf("\n");
	node_print(z);
	printf("\n");
	pq_print(q);

	pq_delete(&q);
	*/
	// END PQ TESTING // YOU NEVER TESTED THE SYMBOLS ARRAY JUST QUICKLY SEE IF IT WORKS
	// CODE TESTING
	/*
	Code c = code_init();
	Code d = code_init();
	uint8_t bit;
	code_push_bit(&c, 0);
	code_push_bit(&c, 1);
	code_push_bit(&c, 0);
	code_push_bit(&c, 1);
	code_push_bit(&c, 0);
	code_push_bit(&c, 1);
	code_push_bit(&c, 1);
	code_print(&c);

	code_pop_bit(&c, &bit);
	code_push_bit(&d, bit);
	
	code_pop_bit(&c, &bit);
	code_push_bit(&d, bit);

	code_pop_bit(&c, &bit);
	code_push_bit(&d, bit);
	
	code_pop_bit(&c, &bit);
	code_push_bit(&d, bit);
	code_pop_bit(&c, &bit);
	code_push_bit(&d, bit);
	code_pop_bit(&c, &bit);
	code_push_bit(&d, bit);
	code_pop_bit(&c, &bit);
	code_push_bit(&d, bit);
	
	
	printf("this is 1 (stg): %u\n", bit);
	code_push_bit(&d, bit);
	code_pop_bit(&c, &bit);
	code_push_bit(&d, bit);
	code_pop_bit(&c, &bit);
	code_push_bit(&d, bit);
	
	
	for (uint32_t i = 0; i < 256; i++) { 
		code_push_bit(&c, 1);
        }
	code_print(&c);
	
	for (uint32_t i = 0; i < 256; i++) {
		code_pop_bit(&c, &bit);
        } 
	code_print(&c);
	code_print(&d);
	code_print(&d);
	*/
	// END CODE TESTING
	// IO TESTING
	uint8_t buf[64];
	int fildes = open("byte.txt", O_RDONLY); ////////// TO READ FROM STDIN  fd = 0 // stdout fd = 1 // stderr fd = 2
	read_bytes(fildes, buf, 8);
	for (uint32_t i = 0; i < 8; i++) {
		printf("%u", buf[i]);
	}
	printf("\n");

	int fildes2 = open("byte2.txt", O_WRONLY);
      	write_bytes(fildes2, buf, 4);
       
	return 0;
}

