#include <stdio.h>
#include <math.h>
#include "mathlib.h"

double Abs(double x) {
double t = 0;
        if (x < 0) {

                t = 0 - x;
                return t;

        } else {
                return x;

        }
}

double my_sqrt(double x) {
    double value = 1.0;
    double EPSILON = 1e-10;
    for (double guess = 0.0; Abs(value - guess) > EPSILON; value = (value + x / value) / 2.0) {
        guess = value;
    }
    return value;
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


int main(void) {
	double d, L;
	L = -.999999;
	d = my_arccos(L);
	printf("Value is: % 1.9lf\n", d);
	return 0;	

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
		} else {		  //n is odd, top value is negative
			total -= top / bottom; // subtracts from total
		}
	prev_k = k;	
	}
	return total;	
}


double my_cos(double x) {
        double total = 0;
        double zero = 0;
        double top = 1;
        double bottom = 1;
        int prev_k = 0;
        for (int n = 0; n < 11; n += 1) {
                int k = (2 * n);

                for (int c = k; c > prev_k; c -= 1) {
                        bottom *=c;
                }
                for (; k > prev_k; prev_k += 1) { // x times itself m times (or k times)
                        top *= x;
                }
                if (n%2 == 0) {         // n is even
                        total += top / bottom; // adds to total
                } else {                  //n is odd, top value is negative
                        total -= top / bottom; // subtracts from total
                }
        	prev_k = k;
        }
	return total;
}


double my_arcsin(double x) {
	double zn = x;
	double new_zn;
	double test;
	double EPSILON = 1e-10;

	do {
    		new_zn = zn - ((my_sin(zn) - x) / my_cos(zn)); //Newton Raphson technique (Z_n+1 = zn....)

		test = Abs(new_zn) - Abs(zn);
		printf("zn = %1.9lf, new_zn = %1.9lf test = %1.9lf\n", zn, Abs(new_zn), Abs(test));
    		zn = new_zn;
		
    	} while (test > EPSILON);

	return new_zn;
}

double my_arccos(double x) {

	return (M_PI/2) - my_arcsin(x);

}

double my_arctan(double x) {
	double t;
	t = ( x/my_sqrt( (x*x) + 1) );
	return my_arcsin(t);
}


