#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <gmp.h>
#include "numtheory.h"
#include "randstate.h"
#include "rsa.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#define OPTIONS "i:o:n:vh"

int main(int argc, char **argv) {
        int opt = 0;
        uint8_t v = 0;
        mpz_t n, d;
        mpz_inits(n, d, NULL);
        char help[] = "Usage: ./keygen [options]\n  ./keygen-dist generates a public / private key pair, placing the keys into the public and private\n  key files as specified below. The keys have a modulus (n) whose length is specified in\n  the program options.\n    -s <seed>   : Use <seed> as the random number seed. Default: time()\n    -b <bits>   : Public modulus n must have at least <bits> bits. Default: 1024\n    -i <iters>  : Run <iters> Miller-Rabin iterations for primality testing. Default: 50\n    -n <pbfile> : Public key file is <pbfile>. Default: rsa.pub\n    -d <pvfile> : Private key file is <pvfile>. Default: rsa.priv\n    -v          : Enable verbose output.\n    -h          : Display program synopsis and usage.\n";
        char *priv_file = (char *)malloc(sizeof(char) * 9);
        char *new_priv_file;
        FILE *pvfile;
        FILE *output_file;
        FILE *cipher_file;
        char *cipher_str = (char *)malloc(sizeof(char));
        char *cipher_name;
        char *output_str = (char *)malloc(sizeof(char));
        char *output_name;
        strcpy(priv_file, "rsa.priv");
        while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
                switch(opt) {
                case 'v':
                        v = v | (1 << 0);
                        break;
                case 'h':
                        v = v | (1 << 1);
                        break;
                case 'n':
                        new_priv_file = (char *)realloc(priv_file, strlen(optarg));
                        strcpy(new_priv_file, optarg);
                        priv_file = new_priv_file;
                        break;
                case 'i':
                        cipher_name = (char *)realloc(cipher_str, strlen(optarg));
                        strcpy(cipher_name, optarg);
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
                exit(1);
        }

        pvfile = fopen(priv_file, "r");
        rsa_read_priv(n, d, pvfile);

        if ((v | (1 << 2)) == v) { // if v or 0100 == v there is a file to specify
                cipher_file = fopen(cipher_name, "r");
        }

        if ((v | (1 << 3)) == v) {
                output_file = fopen(output_name, "w");
        }

        if (((v | (1 << 2)) == v) && ((v | (1 << 3)) == v)) { // if message and output
                rsa_decrypt_file(cipher_file, output_file, n, d);
        } else if (((v | (1 << 2)) != v) && ((v | (1 << 3)) == v)) { // if infile and output
                rsa_decrypt_file(stdin, output_file, n, d);
        } else if (((v | (1 << 2)) == v) && ((v | (1 << 3)) != v)) { // if message amd printf
                rsa_decrypt_file(cipher_file, stdout, n, d);
        } else { // if in_file and printf
                rsa_decrypt_file(stdin, stdout, n, d);
        }
	
	uint64_t str_n = mpz_sizeinbase(n, 2);
        uint64_t str_d = mpz_sizeinbase(d, 2);

        if ((v | (1 << 0)) == v) { // only when v == 1 verbose  if v < 1 or 0 its chill
                gmp_fprintf(stderr, "n - modulus (%lu bits): %Zd\nd - private exponent (%lu bits): %Zd\n", str_n, n, str_d, d);
	}
        free(output_str);
        free(cipher_str);
        free(priv_file);
        mpz_clears(n, d, NULL);
        return 0;
}
