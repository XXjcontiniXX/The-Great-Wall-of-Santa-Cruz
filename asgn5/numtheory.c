#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <gmp.h>
#include "numtheory.h"
#include "randstate.h"
#include <math.h>
#include <assert.h>

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
	mpz_inits(v, p, m, new_v, new_p, new_d, NULL);
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



bool is_prime(mpz_t n, uint64_t iters) {
	//initing
	uint64_t s = 1;
	mpz_t odd, rand, temp_y, y, a, pre_r, c, r, remainder, n_, m, two;
	mpz_inits(odd, rand, temp_y, y, a, pre_r, c, r, remainder, n_, m, two, NULL);
	//
	mpz_sub_ui(n_, n, 1);
	mpz_set_ui(two, 2);
	//
 	mpz_mod(odd, n, two); //  odd = n mod 2  
        if (!mpz_cmp_ui(odd, 0)) {
        	return 0;
        }

	while (true) {
		mpz_pow_ui(m, two, s); //  always m = 2^s TODO MAYBE find a diff way to xponentiate
		mpz_mod(remainder, n_, m); // remainder = n - 1 (mod m^s)
		gmp_printf("%Zd is dividend %Zd is divisor remainder is %Zd and s is %d\n", n_, m, remainder, s);
		if (!mpz_cmp_ui(remainder, 0)) { // remainder == 0
			mpz_fdiv_q(r, n_, m);
			mpz_mod(c, r, two);
			if (!mpz_cmp_ui(c, 1)) {
				//gmp_printf("n-1 is: %Zd /// n-1 mod %Zd = %Zd. r = %Zd and s is %d\n", n_, m, remainder, r, s);
				break;
			}
		}
			
		s += 1;
	}
	
	for (uint64_t i = 1; i <= iters; i++) {
		printf("uh oh");
		mpz_urandomm(rand, state, n_);
		if (!(mpz_cmp_ui(rand, 2) < 0)) { // if rand < 2
			mpz_set(a, rand); // a = rand
			pow_mod(y, a, r, n); // y = pow_mod(a, r, n)
			if (mpz_cmp_ui(y, 1) != 0 && mpz_cmp(y, n_) != 0) { // y != 1 y != n-1
				uint64_t j = 1;
				while (j <= s - 1 && mpz_cmp(y, n_) != 0) {
					mpz_set(temp_y, y);
					pow_mod(y, temp_y, two, n);
					if (!mpz_cmp_ui(y, 1)) { // y == 1
						return false;
					}
					j += 1;
				}
				if (mpz_cmp(y, n_) != 0) {
					return false;
				}
			}	
		} else {
			i -= 1;
		}
	
	}
	return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
	mpz_urandomb(p, state, bits);
	while (!is_prime(p, iters)) {
		mpz_urandomb(p, state, bits);
		gmp_printf("prime: %Zd\n", p);
	}
	gmp_printf("prime: %Zd\n", p);
	return;
}
















