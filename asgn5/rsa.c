#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "numtheory.h"
#include "randstate.h"
#include "rsa.h"
#include <stdbool.h>
#include <stdint.h>

void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters) {
	printf("/// rsa_make_pub ///\n");
	uint64_t p_bits = random();
	uint64_t q_bits;
	//mpz_t test, lambdan, pre_e, ee, gcd_e, pp, qq;
	//mpz_inits(test, lambdan, pre_e, ee, gcd_e, pp, qq, NULL);
	
	if ((nbits % 4) && !(p_bits % (nbits/2))) { // if n_bits not divisble by 4 and random # is 0
		p_bits = (nbits/4) + (nbits % 4);
	}else{
		p_bits = (p_bits % (nbits/2)) + nbits/4;  // random in range 0-n_bits/2 then add n_bits/4
	}
	mpz_set_ui(e, 65537);
	
	q_bits = nbits - p_bits; // q_bits is the difference
	make_prime(p, p_bits, iters); // p holds a random number of p bits
	make_prime(q, q_bits, iters); // q holds a random number of q bits
	mpz_mul(n, p, q); // n hold pq
	gmp_printf("n = %Zd\n", n);
	/*
	mpz_sub_ui(pp, p, 1);
	mpz_sub_ui(qq, q, 1);	
	//gmp_printf("before gcd pp = %Zd qq = %Zd\n", pp, qq);
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
	mpz_set(e, pre_e);*/
	//gmp_printf("e = %Zd ... lambdan = %Zd\n", e, lambdan);
	//mpz_clears(test, lambdan, pre_e, gcd_e, pp, qq, NULL);
	// testing for mpz_make_priv
	return;

}

void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {	
	printf("/// rsa_write_pub ///\n");
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
	printf("/// rsa_make_priv ///\n");
	mpz_t pp, qq, ee;
	mpz_inits(pp, qq, ee, NULL);
	mpz_sub_ui(pp, p, 1);
        mpz_sub_ui(qq, q, 1);
        //gmp_printf("before gcd pp = %Zd qq = %Zd\n", pp, qq);
        //gcd(gcd_e, pp, qq);
        mpz_mul(ee, pp, qq);
        //mpz_fdiv_q(lambdan, ee, gcd_e);	
	mod_inverse(d, e, ee); 
	gmp_printf("d = %Zd .. e = %Zd\n", d, e);
	mpz_clears(ee, pp, qq, NULL);
	return;

}

void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
        printf("/// rsa_write_priv ///\n");
	gmp_printf("%Zd, %Zd\n", n, d);
        char * n_str = mpz_get_str(NULL, 16, n);
        char * d_str = mpz_get_str(NULL, 16, d); // found this nice function on an old archive on gmplib.org
        fprintf(pvfile, "%s\n%s\n", n_str, d_str);
        printf("%s %s\n", n_str, d_str);	


}

void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
	printf("/// rsa_read_pub ///\n");
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


void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
	printf("/// rsa_read_priv ///\n");
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

//
// Encrypts an entire file given an RSA public modulus and exponent.
// All mpz_t arguments are expected to be initialized.
// All FILE * arguments are expected to be properly opened.
//
// infile: the input file to encrypt.
// outfile: the output file to write the encrypted input to.
// n: the public modulus.
// e: the public exponent.
//
void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {
	printf("/// rsa_encrypt_file ///\n");
	uint64_t k;
	k = mpz_sizeinbase(n, 2); // the number requires k bits
	k = (k - 1)/8; // k is how many bytes will be in the array
	size_t read = 1;
	mpz_t ct;
	mpz_t m;
	while (read > 0) {
		
		printf("next block\n");
		uint8_t * block = (uint8_t *)malloc(k); // this array will have almost log2(n) bits of space
                *(block + 0) = 0xFF; // zeroth byte of 488 byte malloc is 0xFF
	
		read = fread(block + 1, sizeof(uint8_t), k-1, infile);

		/*for (uint64_t l = 0; l < k-1; l++){I
			printf("text = %s\n", ((block + 1) + l));
			}*/
		if (!(read > 0)) {
			break;
			
		}else{
		mpz_inits(ct, m, NULL);
		mpz_import(m, k, 1, sizeof(*(block + 0)), 1, 0, block);
		rsa_encrypt(ct, m, e, n);
		char * ct_str = mpz_get_str(NULL, 16, ct);
		fprintf(outfile, "%s\n", ct_str); // open file in append mode "a"
		free(block);
		mpz_clears(ct, m, NULL);
		}
	}

	return;
}

//
// Decrypts an entire file given an RSA public modulus and private key.
// All mpz_t arguments are expected to be initialized.
// All FILE * arguments are expected to be properly opened.
//
// infile: the input file to decrypt.
// outfile: the output file to write the decrypted input to.
// n: the public modulus.
// d: the private key.
//

void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {
	printf("/// rsa_decrypt_file ///\n");
        uint64_t k;
        k = mpz_sizeinbase(n, 2); // the number requires k bits
        k = (k - 1)/8; // k is how many bytes will be in the array
	uint64_t k_size = k;
        uint8_t size = k; 
	mpz_t m, c;
	mpz_inits(c, m, NULL);
	char *buffer = (char *)malloc(sizeof(size));
         // this array will have almost log2(n) bits of space
                                                        // but we dont need all of it cuz the ct must be < n
	
        while (getline(&buffer, &k_size, infile) != -1) {

		uint8_t * block = (uint8_t *)malloc(k_size); // this array will have almost log2(n) bits of space
                printf("new line\n");		// but we dont need all of it cuz the ct must be < n
		printf("hex to convert n decrypt %s\n", buffer);
		mpz_set_str(c, buffer, 16); // c is an mpz_t
		rsa_decrypt(m, c, d, n);
		mpz_export(block, &k_size, 1, sizeof(*(block + 0)), 1, 0, m);
		for (uint64_t i = 1; i < k_size; i++) {
			fprintf(outfile, "%c", *(block + i));
                        printf("printing %c to file\n", *(block + i));
			//printf(".");
                        //printf("%c", *(block + i));
                }	
        	free(block);
        }
	mpz_clears(m, c, NULL);
        return;
}	




void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
	pow_mod(c, m, e, n);
}

void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
	pow_mod(m, c, d, n);
}
