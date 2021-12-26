#include <gmp.h>
#include "randstate.h"
#include "rsa.h"
#include <sys/stat.h>
#include "numtheory.h"
#define OPTIONS "i:o:n:vh"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdbool.h>
int main(int argc, char **argv) {
    int opt = 0;
    FILE *infile = stdin; //set in to standard in
    FILE *outfile = stdout; //set out to standard out
    FILE *rsapriv = fopen("rsa.priv", "r"); //open the private for reading
    bool verbose = false; //set verbose to false
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': infile = fopen(optarg, "r"); break; //open the infile for reading
        case 'o': outfile = fopen(optarg, "w+"); break; //open the outfile for writing
        case 'n': rsapriv = fopen(optarg, "r"); break; //open the private key file for reading
        case 'v': verbose = true; break; //verbose is true
        case 'h': printf("help message"); break;
        default: return 0;
        }
    }

    mpz_t d, n;
    mpz_inits(d, n, NULL);
    rsa_read_priv(n, d, rsapriv);
    if (verbose) { //if verbose print status
        gmp_printf("n(%lu bits) :  %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("d(%lu bits) :  %Zd\n", mpz_sizeinbase(d, 2), d);
    }
    rsa_decrypt_file(infile, outfile, n, d); //decrypt file
    fclose(rsapriv);
    mpz_clears(n, d, NULL);
}
