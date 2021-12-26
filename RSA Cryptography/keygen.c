#include <gmp.h>
#include "randstate.h"
#include "rsa.h"
#include <sys/stat.h>
#include "numtheory.h"
#define OPTIONS "b:i:n:d:s:vh"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdbool.h>
int main(int argc, char **argv) {
    int opt = 0;
    uint64_t bits = 256; //set bits to default bits
    uint64_t iters = 50; //set iters to default iters
    FILE *pbfile = pbfile = fopen("rsa.pub", "w+"); //open the rsa,pub file
    FILE *pvfile = fopen("rsa.priv", "w+"); //open the private key file
    uint64_t seed = time(NULL); //default seed
    bool verbose = false; //set verbose to false
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) { //iterate through the command line
        switch (opt) { //check which value it is currently on
        case 'b': bits = strtoul(optarg, NULL, 10); break; //set bits to optarg
        case 'i': iters = strtoul(optarg, NULL, 10); break; //set iters to optarg
        case 'n': pbfile = fopen(optarg, "w+"); break; //set pbfile to optarg
        case 'd': pvfile = fopen(optarg, "w+"); break; //set pvfile to ptarg
        case 's': seed = strtoul(optarg, NULL, 10); break; //get the seed
        case 'v': verbose = true; break; //set verbose to true
        case 'h':
            printf("SYNOPSIS\nGenerates an RSA public/private key pair.\nUSAGE\n./keygen [-hv] [-b "
                   "bits] -n pbfile -d pvfile\nOPTIONS\n-h              Display program help and "
                   "usage.\n-v              Display verbose program output.\n-b bits         "
                   "Minimum bits needed for public key n.\n-c confidence   Miller-Rabin iterations "
                   "for testing primes (default: 50).\n-n pbfile       Public key file (default: "
                   "rsa.pub).\n-d pvfile       Private key file (default: rsa.priv).n-s seed       "
                   "  Random seed for testing."); //print the help message
            break;
        default: break;
        }
    }

    fchmod(fileno(pvfile), 0600); //get the permissions
    randstate_init(seed); //initialize seed
    mpz_t prevd, p, q, n, e, d, s, rop; //values to be used
    mpz_inits(prevd, p, q, n, e, d, s, rop, NULL);
    rsa_make_pub(p, q, n, e, bits, iters); //make the keys
    rsa_make_priv(d, e, p, q); //make the private key
    mpz_set(prevd, d); //set the previous d value
    char *username = getenv("USER"); //set the username
    mpz_set_str(rop, username, 62); //convert to mpz
    rsa_sign(s, rop, d, n); //sign it
    rsa_write_pub(n, e, s, username, pbfile); //write the public key
    mpz_set(d, prevd); //set the d to prevd
    rsa_write_priv(n, d, pvfile); //write the private value
    if (verbose) { //if verbosse print out all stats
        printf("username   : %s\n", username);
        gmp_printf("s(%lu bits)   : %Zd\n", mpz_sizeinbase(s, 2), s);
        gmp_printf("p(%lu bits)   : %Zd\n", mpz_sizeinbase(p, 2), p);
        gmp_printf("q(%lu bits)   : %Zd\n", mpz_sizeinbase(q, 2), q);
        gmp_printf("n(%lu bits)   : %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("e(%lu bits)   : %Zd\n", mpz_sizeinbase(e, 2), e);
        gmp_printf("d(%lu bits)   : %Zd\n", mpz_sizeinbase(prevd, 2), prevd);
    }
    fclose(pbfile);
    fclose(pvfile);
    randstate_clear();
    mpz_clears(prevd, p, q, n, e, d, s, rop, NULL);
}
