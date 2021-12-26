#include "numtheory.h"
#include "randstate.h"
#include "rsa.h"
#include <gmp.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#define OPTIONS "-hi:o:n:v"
int main(int argc, char **argv) {
    int opt = 0;
    bool verbose = false; //set verbose to false
    FILE *infile = stdin; //set standard in
    FILE *outfile = stdout; //set standard out
    FILE *pbfile = fopen("rsa.pub", "r"); //open public file

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': //if h print the print message
            printf("SYNOPSIS\n   Encrypts data using RSA encryption.\n   Encrypted data is "
                   "decrypted by "
                   "the decrypt program.\n\nUSAGE\n   ./encrypt [-hv] [-i infile] [-o outfile] -n "
                   "pubkey\n\nOPTIONS\n   -h              Display program help and usage.\n   -v   "
                   "        "
                   "   Display verbose program output.\n   -i infile       Input file of data to "
                   "encrypt "
                   "(default: stdin).\n   -o outfile      Output file for encrypted data (default: "
                   "stdout).\n   -n pbfile       Public key file (default: rsa.pub).\n");
            break;
        case 'i': infile = fopen(optarg, "r"); break; //open the infile and read
        case 'o': outfile = fopen(optarg, "w+"); break; //open outfile and write
        case 'n': pbfile = fopen(optarg, "r"); break; //open public file
        case 'v': verbose = true; break; //set verbose to true
        default: return 0;
        }
    }

    mpz_t n, e, s, preve, prevn, user;
    char username[512]; //set username size
    mpz_inits(n, e, s, preve, prevn, user, NULL);

    rsa_read_pub(n, e, s, username, pbfile); //read public file
    mpz_set_str(user, username, 62); //convert to mpz
    mpz_set(preve, e);
    mpz_set(prevn, n);
    if (rsa_verify(user, s, e, n) == false) { //verify the user and if false, error
        fprintf(stderr, "Error : invalid user");
        return 0;
    }

    mpz_set(e, preve);
    mpz_set(n, prevn);
    rsa_encrypt_file(infile, outfile, n, e);
    if (verbose) { //if verbose print stats
        printf("username : %s\n", username);
        gmp_printf("s(%lu)  : %Zd\n", mpz_sizeinbase(s, 2), s);
        gmp_printf("n(%lu)  : %Zd\n", mpz_sizeinbase(prevn, 2), prevn);
        gmp_printf("e(%lu)  : %Zd\n", mpz_sizeinbase(preve, 2), preve);
    }
    mpz_clears(n, e, s, preve, prevn, user, NULL);

    return 0;
}
