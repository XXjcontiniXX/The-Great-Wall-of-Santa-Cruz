// clang-format off
#include <stdio.h>
#include <gmp.h>
// clang-format on
#include "numtheory.h"
#include "randstate.h"
#include "rsa.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#define OPTIONS "b:i:n:d:s:vh"

int main(int argc, char **argv) {
  int opt = 0;
  uint8_t v = 0;
  mpz_t name_62, p, q, e, n, s, d;
  mpz_inits(name_62, p, q, e, n, s, d, NULL);
  char help[] =
      "Usage: ./keygen [options]\n  ./keygen generates a public / private "
      "key pair, placing the keys into the public and private\n  key files as "
      "specified below. The keys have a modulus (n) whose length is specified "
      "in\n  the program options.\n    -s <seed>   : Use <seed> as the random "
      "number seed. Default: time()\n    -b <bits>   : Public modulus n must "
      "have at least <bits> bits. Default: 1024\n    -i <iters>  : Run <iters> "
      "Miller-Rabin iterations for primality testing. Default: 50\n    -n "
      "<pbfile> : Public key file is <pbfile>. Default: rsa.pub\n    -d "
      "<pvfile> : Private key file is <pvfile>. Default: rsa.priv\n    -v      "
      "    : Enable verbose output.\n    -h          : Display program "
      "synopsis and usage.\n";
  char *pub_file = (char *)malloc(sizeof(char) * 8);
  char *priv_file = (char *)malloc(sizeof(char) * 9);
  char *new_pub_file;
  char *new_priv_file;
  int pvfilder;
  FILE *pbfile;
  FILE *pvfile;
  strcpy(pub_file, "rsa.pub");
  strcpy(priv_file, "rsa.priv");
  uint64_t iters = 50;
  uint64_t nbits = 1024;
  uint64_t seed = (uint64_t)time(NULL);
  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {
    case 'b': // nbits
      nbits = strtol(optarg, NULL, 10);
      if (nbits < 50 || nbits > 4096) {
        fprintf(stderr,
                "./keygen: -b option must be between 50 and 4096 inclsuive\n");
        v = v | (1 << 3); // 4th bit is nbits err
      }
      break;
    case 'i': // Miller iters
      iters = strtol(optarg, NULL, 10);
      if (iters < 1 || iters > 500) {
        fprintf(stderr,
                "./keygen: -i option must be between 1 and 500 inclsuive\n");
        v = v | (1 << 2); // third bit is iters err
      }
      break;
    case 'n': // *.pub
      new_pub_file = (char *)realloc(pub_file, strlen(optarg));
      strcpy(new_pub_file, optarg);
      pub_file = new_pub_file;
      break;
    case 'd': // *.priv
      new_priv_file = (char *)realloc(priv_file, strlen(optarg));
      strcpy(new_priv_file, optarg);
      priv_file = new_priv_file;
      break;
    case 's': // seed
      seed = strtoul(optarg, NULL, 10);
      break;
    case 'v':           // verbose
      v = v | (1 << 0); // first bit is verbose output
      break;
    case 'h':           // stderr synopsis and usage
      v = v | (1 << 1); // second bit is -h output
      break;
    default:
      v = v | (1 << 4); // 5th bit is bad option
      break;
    } // switch
  }   // while
  // printf("v = %d\n", v);
  if (v > 1) { // if ann error gets caught here
    fprintf(stderr, "%s", help);
    exit(1);
  }
  pbfile = fopen(pub_file, "w");
  pvfile = fopen(priv_file, "w");

  if (pbfile == NULL || pvfile == NULL) {
    fprintf(stderr, "./keygen: [ERROR] pointer to key file is NULL.\n");
    exit(1);
  }
  pvfilder = fileno(pvfile); // found file num
  fchmod(pvfilder, 0600);    // set that file to 600
  randstate_init(seed);      // inits w/ seed
  rsa_make_pub(p, q, n, e, nbits, iters);
  rsa_make_priv(d, e, p, q);
  char *username = getenv("USER");
  mpz_set_str(name_62, username, 62);
  rsa_sign(s, name_62, d, n);
  rsa_write_pub(n, e, s, username, pbfile);
  rsa_write_priv(n, d, pvfile);
  uint64_t str_s = mpz_sizeinbase(s, 2);
  uint64_t str_p = mpz_sizeinbase(p, 2);
  uint64_t str_q = mpz_sizeinbase(q, 2);
  uint64_t str_n = mpz_sizeinbase(n, 2);
  uint64_t str_e = mpz_sizeinbase(e, 2);
  uint64_t str_d = mpz_sizeinbase(d, 2);
  if (v == 1) { // only when v == 1 verbose  if v < 1 or 0 its chill
    gmp_fprintf(
        stderr,
        "username: %s\nuser signature (%lu bits): %Zd\np (%lu bits): %Zd\nq "
        "(%lu bits): %Zd\nn - modulus (%lu bits): %Zd\ne - public exponent "
        "(%lu bits): %Zd\nd - private exponent (%lu bits): %Zd\n",
        username, str_s, s, str_p, p, str_q, q, str_n, n, str_e, e, str_d, d);
  }
  fclose(pbfile);
  fclose(pvfile);
  mpz_clears(name_62, p, q, e, n, s, d, NULL);
  free(pub_file);
  free(priv_file);
  randstate_clear();
  return 0;
}
