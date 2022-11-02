#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "numtheory.h"
#include "randstate.h"
#include <stdbool.h>
#include <stdint.h>


int main (void) {
	/*mpz_t oo, aa, dd, nn;
	mpz_init(aa);
	mpz_init(dd);
	mpz_init(nn);
	mpz_init(oo);
	mpz_set_ui(aa, 3);
	mpz_set_ui(dd, 128);
	mpz_set_ui(nn, 7);
	
	pow_mod(oo, aa, dd, nn);
	*/
	/*
	mpz_t b, f, g;
	mpz_set_ui(f, 30);
	mpz_set_ui(b, 45);
	gcd(g, f, b); 
*/ /*
 	mpz_t o, a, n;
	mpz_inits(o, a, n, NULL);
        mpz_set_ui(a, 24);
        mpz_set_ui(n, 4294967295);
        mod_inverse(o, a, n);
*//*
	mpz_t crack;
	mpz_init(crack);
	randstate_init(613);	
	mpz_set_ui(crack, 1031712);
	printf("%d\n", is_prime(crack,200));
*/
	mpz_t p;
	mpz_init(p);
	randstate_init(611);
	make_prime(p, 20, 200);

}
