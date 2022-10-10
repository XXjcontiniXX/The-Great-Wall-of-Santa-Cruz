#include <stdio.h>
#include <unistd.h>
#include "mathlib.c"
#define OPTIONS "ascSCTl"


void one (int *rtc) {
	int something = 1; // swap value
	int *uno;	// pointer type var that harbors *something*'s memory address
	uno = &something;
	*rtc = *uno; // the value at memory address rtc is now the value at memory adress uno because pointers point to the value at the adress
}


int main(int argc, char **argv) {
        int opt = 0;
	int test = 1;
	int call[7] = {0,0,0,0,0,0,0};
//	while((prim = getopt(argc, argv, OPTIONS)) != -1) {
//		if (prim == 'a') {
//		zero(&test);
//		printf("prim = %d and test = %d\n", prim, test);
//		}
//	}
//
//	for (int i = 0; i < argc; i += 1) {
//		printf("argv[%d] = %s or directly %d\n", i, argv[i], *argv[i]);
//	}
//
//
//	printf("test outside scope = %d\n", test);
	while ((opt = getopt(argc, argv, OPTIONS)) != -1 && test) {
		
		switch(opt) {
		case 's':
			one(&call[0]);
			break;
		case 'c':
			one(&call[1]);
			break;
                case 'S':
			one(&call[2]);
                        break;
                case 'C':
			one(&call[3]);
                        break;
                case 'T':
                        one(&call[4]);
			break;
                case 'l':
			one(&call[5]);
                        break;	
                case 'a':
			one(&call[6]);
                        break;	
		}

	}
	if (call[6] == 1) {
		printf("run all\n");
	
	}else{
		if (call[0] == 1){
			printf("my sin\n");
		}	for (double i = 0; i <= 
		if (call[1] == 1) {
			printf("my cosin\n");
		}
	       	if (call[2] == 1) {
			printf("my arcsin\n");
		}
	       	if (call[3] == 1) {
			printf("my arccos\n");
		}
	       	if (call[4] == 1) {
			printf("my arctan\n");
		}
	       	if (call[5] == 1) {
			printf("my log\n");
		}
	
	
	}
	return 0;
}

