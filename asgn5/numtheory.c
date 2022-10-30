#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <gmp.h>
#include "numtheory.h"


void gcd(mpz_t d, mpz_t a, mpz_t b) {
	mpz_t t;
	//
	mpz_t L;
	mpz_init(L);
	mpz_set_ui(L, 1);
	//mpz_t ans;
	//
	while (mpz_cmp_ui(b, 0)) { // while b != 0
		mpz_set(t, b);  // t = b
		mpz_mod(b ,a, b); // b = a (mod b) // could be an issue with setting b += foo
		mpz_set(a, t);
	}	
	mpz_set(d, a); // d = a // return d
	//
	//mpz_fdiv_q_ui(ans, L, 2);
	gmp_printf("answer is %Zd\n", L);
	//
}


void pow_mod(mpz_t o, mpz_t a, mpz_t d, mpz_t n) {
	mpz_t v, p, m, new_v, new_p, new_d; // v and p exist
	mpz_init(v);
	mpz_init(p);
	mpz_init(m);
	mpz_init(new_v);
	mpz_init(new_p);
	mpz_init(new_d);
	mpz_set_ui(v, 1); // v = 1
	mpz_set(p, a); // p = a
	while (mpz_cmp_ui(d, 0) > 0) { // d > 0
		mpz_mod_ui(m, d, 2); // m = d % 2
		if (!mpz_cmp_ui(m, 1)) { // if odd
			mpz_mul(new_v, v, p); // new_v = v * p
			mpz_mod(v, new_v, n); // v = new_v mod n
		}
		mpz_mul(new_p, p, p); // new_p = p*p
		mpz_mod(p, new_p, n); // p = new_p mod n
		mpz_set(new_d, d); // d = new_d
		mpz_fdiv_q_ui(d, new_d, 2); // d = new_d/2
	}
	mpz_set(o, v);
	gmp_printf("%Zd is an mpz\n", o);

}

void mod_inverse(mpz_t o, mpz_t a, mpz_t n) {
	mpz_t f, q_t_rp, new_tp, q_t_tp, new_rp, q, t, t_prime, r, r_prime;
	mpz_inits(f, q_t_rp, new_tp, q_t_tp, new_rp, q, t, t_prime, r, r_prime, NULL);
 	mpz_set(r, n); // r = n
	mpz_set(r_prime, a); //r' = a
	mpz_set_ui(t, 0);
	mpz_set_ui(t_prime, 1);
	
	while (mpz_cmp_ui(r_prime, 0)) { // while r_prime != 0 
		
		mpz_fdiv_q(q, r, r_prime);

		mpz_mul(q_t_rp, q, r_prime); //q_t_rp = q * r'
		mpz_sub(new_rp, r, q_t_rp); // new_rp = r - q * r'
		mpz_set(r, r_prime); // r = r'
		mpz_set(r_prime, new_rp); // r' = r - q * r'
		
		mpz_mul(q_t_tp, q, t_prime); // q_t_tp = q * t'
		mpz_sub(new_tp, t, q_t_tp); // new_tp = t - q * t'
		mpz_set(t, t_prime); // t = t'
		mpz_set(t_prime, new_tp); // t' = t - q * t'	
	}
	if ( mpz_cmp_ui(r, 1) > 0 ) {  // r > 1
		mpz_set_ui(t, 0);
		gmp_printf("mod inverse of A = %Zd mod = %Zd is %Zd\n", a, n, o);
		return;	
	} else if (mpz_cmp_ui(t, 0) < 0) { // 
		mpz_add(f, t, n);
		mpz_set(t, f);
	}
	mpz_set(o, t);
	gmp_printf("mod inverse of A = %Zd mod = %Zd is %Zd\n", a, n, o);

}
