#include <stdio.h>
#include <math.h>
#include "mathlib.h"



int main(void) {
	double d, L;
	L = 3;
	d = my_sin(L);
	printf("Value is: %f\n", d);
	return 0;	

}

static double Exp(double x) {
	double t = 1.0;
	double y = 1.0;
	for (double k = 1.0; t > .0000000001; k += 1.0) {
		t *= x / k;
		y += t;
	}
	return y;
}




double my_sin(double x) {
	double total = 0;
	double zero = 0;
	double top = 1;
	double bottom = 1;
	int prev_k = 0;
	for (int n = 0; n < 11; n += 1) {
		int k = (2 * n) + 1;
		for (int c = k; c > prev_k; c -= 1) {
			bottom *=c;
		}
		for (; k > prev_k; prev_k += 1) { // x times itself m times (or k times) 
			top *= x;
		}
		if (n%2 == 0) {  	// n is even
			total += top / bottom; // adds to total
			printf("neg this is top: %f, n = %d, k = %d, prev_k = %d\n", top, n, k, prev_k);
		} else {		  //n is odd, top value is negative
			total -= top / bottom; // subtracts from total
		        printf("neg this is top: %f, n = %d, k = %d, prev_k = %d\n", top, n, k, prev_k);
		}
	prev_k = k;	
	}
return total;	
}

