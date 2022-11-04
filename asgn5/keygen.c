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
	mpz_set_ui(m, 1000000); // m the message is 1000000 
	randstate_init(631);
	rsa_make_pub(p, q, n, e, 50, 200);
	rsa_make_priv(d, e, p, q);
	rsa_sign(s, m, d, n);
	char *username = getenv("USER");
	pbfile = fopen("rsa.pub", "w");
	printf("%d\n", rsa_verify(m, s, e, n));
	rsa_write_pub(n, e, s, username, pbfile);


}
