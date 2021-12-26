#include <stdint.h>
#include <gmp.h>
#include "randstate.h"
gmp_randstate_t state; //initialize external variable seed for the state
void randstate_init(uint64_t seed) { //randstate_init() initializes a seed
    gmp_randinit_mt(state); //initialize the state
    gmp_randseed_ui(state, seed); //initialize the state with the seed
}
void randstate_clear(void) { //randstate clear clears the state's memory
    gmp_randclear(state); //clear the state
}
