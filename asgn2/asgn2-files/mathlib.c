#include <stdio.h>
#include "mathlib.h"



int main(void) {
	double d, L;
	L = 3.14;
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
	double top = 1;
	double bottom = 1;
	double total = 0;
	for (int n = 0; n < 17; n += 1) {
		int k = (2 * n) + 1;
		if (n % 2 == 0) {  	  // n is even
			int e1 = (2 * n) + 1;  // to the e power
			for (int m = 0; m < k; m += 1) { // x times itself m times (or e1 times)
				top *= x;
			}
		
		} else {		  //n is odd, top value is negative
			for (int m = 0; m < k - 1; m += 1) {  // x times itself m times (or e2 times)
				top *= x;
				top = top - (top * 2);
				printf("this is top: %f", top);
			}	  
		}
		for (int c = k; c > 0; c -= 1) {
			bottom *=c;
		}
		total += top / bottom;
		}
return total;	
}
