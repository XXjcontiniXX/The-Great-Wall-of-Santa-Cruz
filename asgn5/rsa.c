#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "numtheory.h"
#include "randstate.h"
#include "rsa.h"
#include <stdbool.h>
#include <stdint.h>

void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters) {
	uint64_t p_bits = random();
	uint64_t q_bits;
	mpz_t test, lambdan, pre_e, ee, gcd_e, pp, qq;
	mpz_inits(test, lambdan, pre_e, ee, gcd_e, pp, qq, NULL);
	if ((nbits % 4) && !(p_bits % (nbits/2))) { // if n_bits not divisble by 4 and random # is 0
		p_bits = (nbits/4) + (nbits % 4);
	}else{
		p_bits = (p_bits % (nbits/2)) + nbits/4;  // random in range 0-n_bits/2 then add n_bits/4
	}
	q_bits = nbits - p_bits; // q_bits is the difference
	make_prime(p, p_bits, iters); // p holds a random number of p bits
	make_prime(q, q_bits, iters); // q holds a random number of q bits
	mpz_mul(n, p, q); // n hold pq
	
	mpz_sub_ui(pp, p, 1);
	mpz_sub_ui(qq, q, 1);	
	gmp_printf("before gcd pp = %Zd qq = %Zd\n", pp, qq);
	gcd(gcd_e, pp, qq);
	mpz_mul(ee, pp, qq);
	mpz_fdiv_q(lambdan, ee, gcd_e);
	printf("p_bits = %lu ... q_bits = %lu\n", p_bits, q_bits);	
	mpz_urandomb(pre_e, state, nbits); // generatre random number compare w/
	gcd(test, pre_e, lambdan);
	while (mpz_cmp_ui(test, 1)) { //gcd of random 
		mpz_urandomb(pre_e, state, nbits);
		gcd(test, pre_e, lambdan);
	}
	mpz_set(e, pre_e);
	gmp_printf("e = %Zd ... lambdan = %Zd\n", e, lambdan);
	mpz_clears(test, lambdan, pre_e, gcd_e, pp, qq, NULL);
	// testing for mpz_make_priv
	mpz_t d;
	mpz_init(d);
	rsa_make_priv(d, e, p, q); 
	//
	return;

}
/*
void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
	pbfile = fopen("rsa.pub", "w");
	//TODO

}
*/

void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
	mpz_t pp, qq, gcd_e, ee, lambdan;
	mpz_inits(pp, qq, gcd_e, ee, lambdan, NULL);
	mpz_sub_ui(pp, p, 1);
        mpz_sub_ui(qq, q, 1);
        //gmp_printf("before gcd pp = %Zd qq = %Zd\n", pp, qq);
        gcd(gcd_e, pp, qq);
        mpz_mul(ee, pp, qq);
        mpz_fdiv_q(lambdan, ee, gcd_e);	
	mod_inverse(d, e, lambdan); 
	gmp_printf("d = %Zd .. e = %Zd .. lmbda(n) = %Zd\n", d, e, lambdan);
	mpz_clears(lambdan, gcd_e, pp, qq, NULL);
	return;

}
/*
void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {



}

*/
