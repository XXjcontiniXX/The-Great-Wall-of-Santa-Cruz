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
#define OPTIONS "i:o:n:vh"

int main(int argc, char **argv) {
  int opt = 0;
  uint8_t v = 0;
  mpz_t name_62, e, n, s;
  mpz_inits(name_62, e, n, s, NULL);
  char help[] =
      "Usage: ./encrypt [options]\n  ./encrypt encrypts an input file using the specified public key file,  \nwriting the result to the specified output file.\n    -i <infile> : Read input from <infile>. Default: standard input.\n    -o <outfile>: Write output to <outfile>. Default: standard output.\n    -n <keyfile>: Public key is in <keyfile>. Default: rsa.pub.\n    -v          : Enable verbose output.\n    -h          : Display program synopsis and usage.\n";
  char *username = (char *)malloc(sizeof(char));
  char *pub_file = (char *)malloc(sizeof(char) * 8);
  char *new_pub_file;
  FILE *pbfile;
  FILE *output_file;
  FILE *message_file;
  char *message_str = (char *)malloc(sizeof(char));
  char *message_name;
  char *output_str = (char *)malloc(sizeof(char));
  char *output_name;
  strcpy(pub_file, "rsa.pub");
  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {
    case 'v':
      v = v | (1 << 0);
      break;
    case 'h':
      v = v | (1 << 1);
      break;
    case 'n':
      new_pub_file = (char *)realloc(pub_file, strlen(optarg));
      strcpy(new_pub_file, optarg);
      pub_file = new_pub_file;
      break;
    case 'i':
      message_name = (char *)realloc(message_str, strlen(optarg));
      strcpy(message_name, optarg);
      v = v | (1 << 2);
      break;
    case 'o':
      output_name = (char *)realloc(output_str, strlen(optarg));
      strcpy(output_name, optarg);
      v = v | (1 << 3);
      break;
    default:
      v = v | (1 << 1);
      break;
    }
  }
  if (v == (v | (1 << 1))) {
    fprintf(stderr, "%s", help);
    return 1;
  }

  pbfile = fopen(pub_file, "r");
  if (pbfile == NULL) {
      fprintf(stderr, "./encrypt: [ERROR] pointer to key file is NULL.\n");
      return 1;
  }
  rsa_read_pub(n, e, s, username, pbfile);
  mpz_set_str(name_62, username, 62);
  if (!rsa_verify(name_62, s, e, n)) {
    fprintf(stderr, "./encrypt: [ERROR] Signature couldn't be verified.\n");
    return 1; 
  }
  uint64_t str_s = mpz_sizeinbase(s, 2);
  uint64_t str_n = mpz_sizeinbase(n, 2);
  uint64_t str_e = mpz_sizeinbase(e, 2);

  if ((v | (1 << 0)) ==
      v) { // only when v == 1 verbose  if v < 1 or 0 its chill
    gmp_fprintf(stderr,
                "username: %suser signature (%lu bits): %Zd\nn - modulus (%lu "
                "bits): %Zd\ne - public exponent (%lu bits): %Zd\n",
                username, str_s, s, str_n, n, str_e, e);
  }

  if ((v | (1 << 2)) == v) { // if v or 0100 == v there is a file to specify
    message_file = fopen(message_name, "r");
  }

  if ((v | (1 << 3)) == v) {
    output_file = fopen(output_name, "w");
  }

  if (((v | (1 << 2)) == v) && ((v | (1 << 3)) == v)) { // if message and output
    rsa_encrypt_file(message_file, output_file, n, e);
    fclose(output_file);
    fclose(message_file);
  } else if (((v | (1 << 2)) != v) &&
             ((v | (1 << 3)) == v)) { // if infile and output
    rsa_encrypt_file(stdin, output_file, n, e);
    fclose(output_file);
  } else if (((v | (1 << 2)) == v) &&
             ((v | (1 << 3)) != v)) { // if message amd printf
    rsa_encrypt_file(message_file, stdout, n, e);
    fclose(message_file);
  } else { // if in_file and printf.
    rsa_encrypt_file(stdin, stdout, n, e);
  }

  free(output_str);
  free(message_str);
  free(pub_file);
  free(username);
  mpz_clears(name_62, e, n, s, NULL);
  return 0;
}
