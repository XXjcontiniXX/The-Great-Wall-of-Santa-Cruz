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
        mpz_t name_62, e, n, s, m, ct;
        mpz_inits(name_62, e, n, s, m, ct, NULL);
        char help[] = "Usage: ./keygen [options]\n  ./keygen-dist generates a public / private key pair, placing the keys into the public and private\n  key files as specified below. The keys have a modulus (n) whose length is specified in\n  the program options.\n    -s <seed>   : Use <seed> as the random number seed. Default: time()\n    -b <bits>   : Public modulus n must have at least <bits> bits. Default: 1024\n    -i <iters>  : Run <iters> Miller-Rabin iterations for primality testing. Default: 50\n    -n <pbfile> : Public key file is <pbfile>. Default: rsa.pub\n    -d <pvfile> : Private key file is <pvfile>. Default: rsa.priv\n    -v          : Enable verbose output.\n    -h          : Display program synopsis and usage.\n";
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
                switch(opt) {
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
		exit(1);
	}

	pbfile = fopen(pub_file, "r");
	rsa_read_pub(n, e, s, username, pbfile);
        mpz_set_str(name_62, username, 62);
	if (!rsa_verify(name_62, s, e, n)) {
                        fprintf(stderr, "./encrypt: [ERROR] Signature couldn't be verified.\n");
                        exit(1);
	}
	
	if ((v | (1 << 2)) == v) { // if v or 0100 == v there is a file to specify
		message_file = fopen(message_name, "r");
	}
	
	if ((v | (1 << 3)) == v) {
		output_file = fopen(output_name, "w");
	}

	if (((v | (1 << 2)) == v) && ((v | (1 << 3)) == v)) { // if message and output
		rsa_encrypt_file(message_file, output_file, n, e);
	} else if (((v | (1 << 2)) != v) && ((v | (1 << 3)) == v)) { // if infile and output
		rsa_encrypt_file(stdin, output_file, n, e);
	} else if (((v | (1 << 2)) == v) && ((v | (1 << 3)) != v)) { // if message amd printf
		rsa_encrypt_file(message_file, stdout, n, e);
	} else { // if in_file and printf
		rsa_encrypt_file(stdin, stdout, n, e);
	}
	free(output_str);
	free(message_str);
	free(pub_file);
	mpz_clears(name_62, e, n, s, m, ct, NULL);
	return 1;
}
