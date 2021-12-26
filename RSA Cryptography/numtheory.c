#include <stdbool.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <gmp.h>
#include "randstate.h"
#include "numtheory.h"
#include <math.h>
#include <time.h>
void gcd(mpz_t g, mpz_t a, mpz_t b) { //gcd calculates the gcd of two values
    mpz_t t, prevb; //prevb is a temporary variable that stores the value of b to prevent overriding
        //t stores the computed divisor temporarily until it is set to a and then g
    mpz_inits(t, prevb, NULL); //initialize variables
    while (mpz_cmp_ui(b, 0) != 0) { //while the value of b is not equal to zero, loop

        mpz_set(t, b); //set t to b
        mpz_set(prevb, b); //set prevb to b
        mpz_mod(b, a, prevb); //get a mod b
        mpz_set(a, t); //set t to a
    }
    mpz_set(g, a); //set g to the computed gcd
    mpz_clears(t, prevb, NULL); //clear variables
    return;
}
void mod_inverse(
    mpz_t o, mpz_t a, mpz_t n) { //mod inverse calculates the modular inverse of a value
    mpz_t r, rprime, t, tprime, zero, one, q, prevr, mul, diff, prevt, difft,
        mult; //initialize variabless
    mpz_inits(r, rprime, t, tprime, zero, one, q, prevr, mul, diff, prevt, difft, mult,
        NULL); //inititailze them really
    mpz_set(r, n); //set r to n
    mpz_set(rprime, a); //set rprime to a
    mpz_set_si(t, 0); //set t to zero
    mpz_set_si(tprime, 1); //set tprime to one
    mpz_set_si(zero, 0); //set zero to zero
    mpz_set_si(one, 1); //set one to one
    while (mpz_cmp(rprime, zero) != 0) { //while rprime is not equal to zero
        mpz_div(q, r, rprime); //divide r by rprime
        mpz_set(prevr, r); //set the temp variable prevr to r
        mpz_set(r, rprime); //set r to rprime
        mpz_mul(mul, q, rprime); //multiply q by rprime
        mpz_sub(rprime, prevr, mul); //subtract prevr by mul
        mpz_set(prevt, t); //set prevt to t
        mpz_set(t, tprime); //set t to ptprime
        mpz_mul(mult, q, tprime); //multiply q by tprime
        mpz_sub(difft, prevt, mult); //subtract prevt by mult
        mpz_set(tprime, difft); //set tprime to the difference
    }
    if (mpz_cmp_ui(r, 1) > 0) { //if r is greater than one
        mpz_set_si(o, 0); //there is no modular inverse
        mpz_clears(r, rprime, t, tprime, zero, one, q, prevr, mul, diff, prevt, difft, mult,
            NULL); //clears
        return;
    }
    if (mpz_cmp_ui(t, 0) < 0) { //if t is lesss than zero

        mpz_add(t, t, n); //add t by n
    }
    mpz_set(o, t); //set the mod inverse to the out variable
    mpz_clears(
        r, rprime, t, tprime, zero, one, q, prevr, mul, diff, prevt, difft, mult, NULL); //clears
    return;
}

void pow_mod(mpz_t o, mpz_t a, mpz_t d,
    mpz_t n) { //calculates the powermod of a base, an exponent, and a modulus

    mpz_t v, p, zero, one, two, modd, modprod, prod, psquare, prevp,
        divd; //variables used in the funcion
    mpz_inits(
        v, p, zero, one, two, modd, modprod, prod, psquare, prevp, divd, NULL); //initialize them
    mpz_set_si(v, 1); //set v to one
    mpz_set(p, a); //set p to a
    mpz_set_si(zero, 0); //set zero to zero
    mpz_set_si(one, 1); //set one to one
    mpz_set_si(two, 2); //set two to two
    while (mpz_cmp(d, zero) > 0) { //while d is greater than zero
        mpz_mod(modd, d, two); //get the mod of d and two
        if (mpz_cmp(modd, one) == 0) { //if the mod is equal to one
            mpz_mul(prod, v, p); //multiply v by p
            mpz_mod(modprod, prod, n); //get prod mod n
            mpz_set(v, modprod); //set v to modprod
        }
        mpz_set(prevp, p); //set the temp variable prevp to p
        mpz_mul(psquare, p, prevp); //set psquare to p time p
        mpz_mod(p, psquare, n); //get psquare mod n
        mpz_div_ui(divd, d, 2); //divide d by two
        mpz_set(d, divd); //set d to divd
    }
    mpz_set(o, v); //set o to the output
    mpz_clears(v, p, zero, one, two, modd, modprod, prod, psquare, prevp, divd, NULL); //clears
}

bool is_prime(mpz_t n, uint64_t iters) {
    mpz_t etest, a, y, j, r, s, mr, nminusone, nminustwo, sminusone, one,
        two; //variables to be used in the function
    mpz_inits(etest, a, y, j, r, s, mr, nminusone, nminustwo, sminusone, one, two,
        NULL); //initialize them
    if (mpz_cmp_ui(n, 0) == 0) { //if zero
        return false; //false
    }
    mpz_mod_ui(etest, n, 2); //get the mod of n
    if (mpz_cmp_ui(etest, 0) == 0
        && mpz_cmp_ui(n, 2) != 0) { //if zero or n not equal to two return false
        return false;
    }
    if (mpz_cmp_ui(n, 1) == 0) { //if one return false
        return false;
    }
   
    if (mpz_cmp_ui(n, 2) == 0 || mpz_cmp_ui(n, 3) == 0) { //if 2 or 3 return false
        return true;
    }
    mpz_sub_ui(r, n, 1);
    mpz_mod_ui(mr, r, 2);
    while (mpz_cmp_ui(mr, 0) == 0) { //if r is even, loop
        mpz_add_ui(s, s, 1); //add one to the conut
        mpz_divexact_ui(r, r, 2); //if r is divisible by the number, then divide
        mpz_mod_ui(mr, r, 2); //check the mod again
    }
    for (uint64_t i = 1; i < iters; i++) { //for i in range i through iters
        mpz_sub_ui(nminustwo, n, 2);
        mpz_set_ui(a, 2);
        mpz_urandomm(a, state, nminustwo);
        pow_mod(y, a, r, n); //get the power modulus of the random number, r, and the input n number
        mpz_sub_ui(nminusone, n, 1);
        if (mpz_cmp_ui(y, 1) != 0
            && mpz_cmp(y, nminusone) != 0) { //if y is not one or y is not n minus one
            mpz_set_ui(j, 1);
            mpz_sub_ui(sminusone, s, 1);
            while (
                (mpz_cmp(j, sminusone) < 0
                    || mpz_cmp(j, sminusone)
                           == 0) //if j is lesss than s minus one or j is less than s minus one, loop
                && mpz_cmp(y, nminusone) != 0) { //if y is not equal to nminus one
                mpz_set_ui(two, 2);
                pow_mod(y, y, two, n); //get the powermod of y , two and the n value
                if (mpz_cmp_ui(y, 1) == 0) { //if y is one then not prime
                    mpz_clears(
                        etest, a, y, j, r, s, mr, nminusone, nminustwo, sminusone, one, two, NULL);
                    return false;
                }
                mpz_add_ui(j, j, 1);
            }
            if (mpz_cmp(y, nminusone) != 0) { //y not equal to nminus one
                mpz_clears(
                    etest, a, y, j, r, s, mr, nminusone, nminustwo, sminusone, one, two, NULL);
                return false; //return false
            }
        }
    }
    mpz_clears(etest, a, y, j, r, s, mr, nminusone, nminustwo, sminusone, one, two, NULL);
    return true; //if none of the other ifs check out, return true
}
void make_prime(mpz_t p, uint64_t bits, uint64_t iters) { //makes a prime
    mpz_t rp;
    mpz_init(rp);
    while (!is_prime(rp, iters) || (mpz_sizeinbase(rp, 2) < bits)) {
        mpz_urandomb(rp, state, bits);
    }
    mpz_set(p, rp);
    mpz_clear(rp);
    return;
}
