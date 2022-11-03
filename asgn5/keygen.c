#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "numtheory.h"
#include "randstate.h"
#include "rsa.h"
#include <stdbool.h>
#include <stdint.h>


int main(void) {
	mpz_t a, b, c, d;
	mpz_inits(a, b, c, d, NULL);
	randstate_init(901);
	rsa_make_pub(a, b, c, d, 50, 200);


}
