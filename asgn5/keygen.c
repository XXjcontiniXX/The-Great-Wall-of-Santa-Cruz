#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "numtheory.h"
#include "randstate.h"
#include "rsa.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

int main(void) {
	mpz_t s, m, d, p, q, n, e;
	mpz_inits(s, m, d, p, q, n, e, NULL);
	FILE *pbfile;
	FILE *pvfile;
	FILE *ct;
	FILE *message;
	FILE *output;
	ct = fopen("ct.txt", "w");
	fclose(ct);
	output = fopen("output.txt", "w");
	fclose(output);



	mpz_set_ui(m, 1000000); // m the message is 1000000 
	randstate_init(100);
	rsa_make_pub(p, q, n, e, 512, 200);
	rsa_make_priv(d, e, p, q);
	rsa_sign(s, m, d, n);
	char *username = getenv("USER");
	pbfile = fopen("rsa.pub", "w");
	pvfile = fopen("rsa.priv", "w");
	printf("%d\n", rsa_verify(m, s, e, n));
	rsa_write_pub(n, e, s, username, pbfile);
	rsa_write_priv(n, d, pvfile);
	fclose(pbfile);
	fclose(pvfile);
	pbfile = fopen("rsa.pub", "r");
	pvfile = fopen("rsa.priv", "r");
	ct = fopen("ct.txt", "a");
	message = fopen("message.txt", "r");
	rsa_read_pub(n, e, s, username, pbfile); // reads from rsa.pub into n e s 
	rsa_read_priv(n, d, pvfile); // reads from rsa.priv into n and d
	rsa_encrypt_file(message, ct, n, e);
	fclose(ct);
	ct = fopen("ct.txt", "r");
	output = fopen("output.txt", "a");
	rsa_decrypt_file(ct, output, n, d);
	
}
