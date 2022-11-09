// clang-format off
#include <stdio.h>
#include <gmp.h>
// clang-format on
#include "rsa.h"
#include "numtheory.h"
#include "randstate.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits,
                  uint64_t iters) {
  // printf("/// rsa_make_pub ///\n");
  uint64_t p_bits = random();
  uint64_t q_bits;

  if ((nbits % 4) &&
      !(p_bits %
        (nbits / 2))) { // if n_bits not divisble by 4 and random # is 0
    p_bits = (nbits / 4) + (nbits % 4);
  } else {
    p_bits = (p_bits % (nbits / 2)) +
             nbits / 4; // random in range 0-n_bits/2 then add n_bits/4
  }
  mpz_set_ui(e, 65537);

  q_bits = nbits - p_bits;      // q_bits is the difference
  make_prime(p, p_bits, iters); // p holds a random number of p bits
  make_prime(q, q_bits, iters); // q holds a random number of q bits
  printf("make primes are done\n");
  mpz_mul(n, p, q);             // n holds product
  printf("n is done\n"); 
  return;
}

void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
  // printf("/// rsa_write_pub ///\n");
  // gmp_printf("%Zd, %Zd %Zd\n", n, e, s);
  char *n_str = mpz_get_str(NULL, 16, n);
  char *e_str = mpz_get_str(NULL, 16, e);
  char *s_str = mpz_get_str(
      NULL, 16, s); // found this nice function on an old archive on gmplib.org
  fprintf(pbfile, "%s\n%s\n%s\n%s\n", n_str, e_str, s_str, username);
  // printf("%s %s %s %s\n", n_str, e_str, s_str, username);
  return;
}

void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) { pow_mod(s, m, d, n); }

bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
  mpz_t verify;
  mpz_init(verify);
  pow_mod(verify, s, e, n);
  if (!mpz_cmp(verify, m)) {
    mpz_clear(verify);
    return true;
  } else {
    mpz_clear(verify);
    return false;
  }
}

void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
  // printf("/// rsa_make_priv ///\n");
  mpz_t pp, qq, ee;
  mpz_inits(pp, qq, ee, NULL);
  mpz_sub_ui(pp, p, 1);
  mpz_sub_ui(qq, q, 1);
  // gmp_printf("before gcd pp = %Zd qq = %Zd\n", pp, qq);
  // gcd(gcd_e, pp, qq);
  mpz_mul(ee, pp, qq);
  // mpz_fdiv_q(lambdan, ee, gcd_e);
  mod_inverse(d, e, ee);
  // gmp_printf("d = %Zd .. e = %Zd\n", d, e);
  mpz_clears(ee, pp, qq, NULL);
  return;
}

void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
  // printf("/// rsa_write_priv ///\n");
  // gmp_printf("%Zd, %Zd\n", n, d);
  char *n_str = mpz_get_str(NULL, 16, n);
  char *d_str = mpz_get_str(
      NULL, 16, d); // found this nice function on an old archive on gmplib.org
  fprintf(pvfile, "%s\n%s\n", n_str, d_str);
  // printf("%s %s\n", n_str, d_str);
}

void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
  // printf("/// rsa_read_pub ///\n");
  int i = 0;
  size_t len = 0;
  char *key = (char *)malloc(sizeof(char));
  while (getline(&key, &len, pbfile) != 1) {
    // printf("works on some level");
    if (i == 0) {
      if (mpz_set_str(n, key, 16) != 0) {
        fprintf(stderr, "Error: Invalid  public key.\n");
        exit(2);
      }
      i++;
      // gmp_printf("n is %Zd\n", n);
      continue;
    }
    if (i == 1) {
      if (mpz_set_str(e, key, 16) != 0) {
        fprintf(stderr, "Error: Invalid  public exponent.\n");
        exit(2);
      }
      i++;
      // gmp_printf("e is %Zd\n", e);
      continue;
    }
    if (i == 2) {
      if (mpz_set_str(s, key, 16) != 0) {
        fprintf(stderr, "Error: Invalid signature\n");
        exit(2);
      }
      i++;
      // gmp_printf("s is %Zd\n", s);
      continue;
    }
    if (i == 3) {
      username = (char *)realloc(username, strlen(key));
      strcpy(username, key);
      i++;
      // printf("username is %s", username);
      continue;
    }
    free(key);
    break;
  }
}

void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
  // printf("/// rsa_read_priv ///\n");
  int i = 0;
  size_t len = 0;
  char *key = (char *)malloc(sizeof(char));
  while (getline(&key, &len, pvfile) != 1) {
    // printf("works on some level");
    if (i == 0) {
      if (mpz_set_str(n, key, 16) != 0) {
        fprintf(stderr, "Error: Invalid  public key.\n");
        exit(3);
      }
      i++;
      // gmp_printf("n is %Zd\n", n);
      continue;
    }
    if (i == 1) {
      if (mpz_set_str(d, key, 16) != 0) {
        fprintf(stderr, "Error: Invalid  private key.\n");
        exit(3);
      }
      i++;
      // gmp_printf("d is %Zd\n", d);
      continue;
    }
    free(key);
    break;
  }
}

void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {
  // printf("/// rsa_encrypt_file ///\n");
  mpz_t ct, m;
  mpz_inits(ct, m, NULL);
  uint16_t k;
  size_t size;
  uint8_t *block = NULL;
  size = mpz_sizeinbase(n, 2);
  k = ((size - 1) /
       8); // this is the block size: we encrypt this many bytes of data
  block = (uint8_t *)malloc(k * sizeof(uint8_t)); // k bytes long
  *(block + 0) = 0xFF;                            // zerot out of k bytes is 0
  size_t j;
  do {
    *(block + 0) = 0xFF;
    j = fread(block + 1, sizeof(uint8_t), k - 1,
              infile); // reads j bytes, block is filled up to j + 1

    if (j == 0) {
      break;
    }
    if (j < k - 1) { // if didn't fill whole block
      *(block + j) = '\0';
    }
    mpz_import(m, j + 1, 1, sizeof(*(block + 0)), 1, 0, block);
    rsa_encrypt(ct, m, e, n);
    char *ct_str = mpz_get_str(NULL, 16, ct);
    fprintf(outfile, "%s\n", ct_str); // open file in append mode "a"
  } while (j == k - 1);
  free(block);
  mpz_clears(ct, m, NULL);
  return;
}

void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {
  // printf("/// rsa_decrypt_file ///\n");
  mpz_t ct, m;
  mpz_inits(ct, m, NULL);
  uint16_t k;
  size_t *deref_size = 0; // size of hexstring (which is unnecesarry) lands here
  size_t size;
  uint8_t *block = NULL;
  size = mpz_sizeinbase(n, 2);
  k = ((size - 1) /
       8); // this is the block size: we encrypt this many bytes of data
  block = (uint8_t *)malloc((k * sizeof(uint8_t)) + sizeof(uint8_t)); //
  size_t len = 0;
  char *buffer = (char *)malloc(sizeof(char));
  while (getline(&buffer, &len, infile) != -1) {
    // printf("line got");
    *(block + k) = '\0';
    mpz_set_str(ct, buffer, 16); // mpz has the hexstring
    rsa_decrypt(m, ct, d, n);    // m contains the the hextring
    mpz_export(block, deref_size, 1, sizeof(*(block + 0)), 1, 0,
               m); // m to block
    // printf("decrypted block: %s\n", block + 1); //for some
    fprintf(outfile, "%s\n",
            block +
                1); // for some reason accessing the 1 out of bounds (block + k)..
  }
 	 mpz_clears(ct, m, NULL);
  free(block);
  return;
}

void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) { pow_mod(c, m, e, n); }

void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) { pow_mod(m, c, d, n); }
