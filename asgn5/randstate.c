#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <gmp.h>
#include "numtheory.h"
#include "randstate.h"
#include <math.h>

gmp_randstate_t state;
void randstate_init(uint64_t seed) {

	srandom(seed);
	gmp_randinit_mt(state);
	gmp_randseed_ui(state, seed);
	return;
}
