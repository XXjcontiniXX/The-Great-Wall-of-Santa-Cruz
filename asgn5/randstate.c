// clang-format off
#include <stdio.h>
#include <gmp.h>
// clang-format on
#include "randstate.h"
#include "numtheory.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

gmp_randstate_t state;
void randstate_init(uint64_t seed) {
  srandom(seed);
  gmp_randinit_mt(state);
  gmp_randseed_ui(state, seed);
  return;
}

void randstate_clear(void) {
  gmp_randclear(state);
  return;
}
