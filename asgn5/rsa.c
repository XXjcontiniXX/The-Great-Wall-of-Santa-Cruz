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
	return;

}

void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {	
	gmp_printf("%Zd, %Zd %Zd\n", n, e, s);
	char * n_str = mpz_get_str(NULL, 16, n);
	char * e_str = mpz_get_str(NULL, 16, e);
	char * s_str = mpz_get_str(NULL, 16, s); // found this nice function on an old archive on gmplib.org
	fprintf(pbfile, "%s\n%s\n%s\n%s\n", n_str, e_str, s_str, username);
	printf("%s %s %s %s\n", n_str, e_str, s_str, username);
	return;
}


void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
	pow_mod(s, m, d, n);

}

bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
	mpz_t verify;
        mpz_init(verify);
	pow_mod(verify, s, e, n);
	if(!mpz_cmp(verify, m)) {
		mpz_clear(verify);
		return true;
	}else{
		mpz_clear(verify);
		return false;
	}

}

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

void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
        gmp_printf("%Zd, %Zd\n", n, d);
        char * n_str = mpz_get_str(NULL, 16, n);
        char * d_str = mpz_get_str(NULL, 16, d); // found this nice function on an old archive on gmplib.org
        fprintf(pvfile, "%s\n%s\n", n_str, d_str);
        printf("%s %s\n", n_str, d_str);	


}

//
// Reads a public RSA key from a file.
// Public key contents: n, e, signature, username.
// All mpz_t arguments are expected to be initialized.
//
// n: will store the public modulus.
// e: will store the public exponent.
// s: will store the signature.
// username: an allocated array to hold the username.
// pbfile: the file containing the public key
//
void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
	int i = 0;
	size_t size;
	char *key = (char *)malloc(sizeof(size));
	while (getline(&key, &size, pbfile) != 1) {
		//printf("works on some level");
		if (i == 0) {
			if (mpz_set_str(n, key, 16) != 0) {
				printf("Error: Invalid  public key.\n");
			}
			i++;
			gmp_printf("n is %Zd\n", n);
			continue;
		}
		if (i == 1) {
                        if (mpz_set_str(e, key, 16) != 0) {
                                printf("Error: Invalid  public exponent.\n");
                        }
                        i++;
                        gmp_printf("e is %Zd\n", e);
			continue;
		}
		if (i == 2) {
                        if (mpz_set_str(s, key, 16) != 0) {
                                printf("Error: Invalid signature\n");
                        }
                        i++;
                        gmp_printf("s is %Zd\n", s);
			continue;
		}
		if (i == 3) {
			username = key;
                        i++;
                        printf("username is %s", username);
			continue;
		}
		break;
	
	}
}

// Reads a private RSA key from a file.
// Private key contents: n, d.
// All mpz_t arguments are expected to be initialized.
//
// n: will store the public modulus.
// d: will store the private key.
void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
	int i = 0;
        size_t size;
        char *key = (char *)malloc(sizeof(size));
        while (getline(&key, &size, pvfile) != 1) {
                //printf("works on some level");
                if (i == 0) {
                        if (mpz_set_str(n, key, 16) != 0) {
                                printf("Error: Invalid  public key.\n");
                        }
                        i++;
                        gmp_printf("n is %Zd\n", n);
                        continue;
                }
                if (i == 1) {
                        if (mpz_set_str(d, key, 16) != 0) {
                                printf("Error: Invalid  private key.\n");
                        }
                        i++;
                        gmp_printf("d is %Zd\n", d);
                        continue;
		}
		break;
	}
}
