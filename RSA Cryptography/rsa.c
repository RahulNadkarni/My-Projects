#include <stdio.h>

#include <gmp.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "numtheory.h"
#include "randstate.h"
#include "rsa.h"
#include <stdarg.h>
#include <math.h>
void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits,
    uint64_t iters) { //this function makes the keys and generates two random primes
    mpz_t out, totient, pminusone, a, prevtotient, qminusone;
    mpz_inits(out, totient, pminusone, a, qminusone, prevtotient, NULL);
    uint64_t pbits, qbits = 0;
    srand(time(NULL));
    pbits = rand() % (((3 * nbits) / 4) + 1 - (nbits / 4))
            + (nbits / 4); //formula to get the right number of bits to get p
    qbits = nbits - pbits; //q bits should equal the difference of nbits and pbits
    make_prime(p, pbits, iters); //make p
    make_prime(q, qbits + 1, iters); //make q

    mpz_sub_ui(pminusone, p, 1);
    mpz_sub_ui(qminusone, q, 1);
    mpz_mul(n, p, q); //get n by multiplying p and q
    mpz_mul(
        totient, pminusone, qminusone); //get the totient by multiplying p minus one and q minus one
    mpz_set(prevtotient, totient); //set the prev value to the actual value to prevent overridng

    while (mpz_cmp_ui(out, 1)
           != 0) { //while the gcd is not one, continue looping until the right value is found
        mpz_set(totient, prevtotient);
        mpz_urandomb(a, state, nbits);
        mpz_set(e, a);
        gcd(out, a, totient);
    }
    mpz_clears(out, totient, pminusone, a, qminusone, prevtotient, NULL);
}

void rsa_write_pub(
    mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) //writes values to outfile

{
    gmp_fprintf(pbfile, "%Zx\n%Zx\n%Zx\n%s\n", n, e, s, username); //write all values
}

void rsa_read_pub(
    mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) { //reads all values from infile
    gmp_fscanf(pbfile, "%Zx\n%Zx\n%Zx\n%s\n", n, e, s, username); //scan in all values
}

void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) { //makes the private keys
    mpz_t totient, pminusone, qminusone;
    mpz_inits(totient, pminusone, qminusone, NULL);
    mpz_sub_ui(pminusone, p, 1);
    mpz_sub_ui(qminusone, q, 1);
    mpz_mul(totient, pminusone, qminusone); //get the totient
    mod_inverse(d, e, totient); //calculate the mod inverse of e and the totient
    mpz_clears(totient, pminusone, qminusone, NULL);
    return;
}

void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) { //write the private key
    gmp_fprintf(pvfile, "%Zx\n%Zx\n", n, d);
}

void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) { //read the private key
    gmp_fscanf(pvfile, "%Zx\n%Zx\n", n, d);
}

void rsa_encrypt(
    mpz_t c, mpz_t m, mpz_t e, mpz_t n) { //encrypt the messsage using the power mod function
    pow_mod(c, m, e, n);
}

void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) { //encrypts the full file
    uint64_t k;
    k = mpz_sizeinbase(n, 2);
    k -= 1;
    k = floor(k / 8); //calculate the block size k = log2(n)-1/8
    uint8_t *block = (uint8_t *) calloc(k, sizeof(uint8_t)); //dynamically allocate the block
    block[0] = 0xff; //prepend 0xff
    size_t j; //set the size j for the read
    mpz_t m, c; //message and ciphertext
    mpz_inits(m, c, NULL);
    mpz_t preve, prevn;
    mpz_inits(preve, prevn, NULL);
    mpz_set(preve, e); //set prev values
    mpz_set(prevn, n);
    while (!feof(infile)) { //while not the end of the file
        j = fread(block + 1, sizeof(uint8_t), k - 1,
            infile); //read the file and store the bytes in the block
        mpz_import(m, j + 1, 1, sizeof(uint8_t), 1, 0, block); //import the block into the mpz m
        mpz_set(e, preve);
        mpz_set(n, prevn);
        rsa_encrypt(c, m, e, n); //encrypt the file
        gmp_fprintf(outfile, "%Zx\n", c); //write to the outfile
    }
    free(block);
    mpz_clears(m, c, preve, prevn, NULL);
}
void rsa_decrypt(
    mpz_t m, mpz_t c, mpz_t d, mpz_t n) { //decrypts a message by calculating the power mod
    pow_mod(m, c, d, n);
}

void rsa_decrypt_file(
    FILE *infile, FILE *outfile, mpz_t n, mpz_t d) { //decrypts a file and writes to the outfile

    uint64_t k; //calculate k the same way done in encrypt
    k = mpz_sizeinbase(n, 2);
    k -= 1;
    k = floor(k / 8);
    size_t j;
    mpz_t prevd, prevn;
    mpz_inits(prevd, prevn, NULL);
    mpz_set(prevd, d);
    mpz_set(prevn, n);
    uint8_t *block = (uint8_t *) calloc(k, sizeof(uint8_t)); //allocate space for the block
    mpz_t c, m;
    mpz_inits(c, m, NULL);
    while (gmp_fscanf(infile, "%Zx\n", c) != -1) { //while there are still values in the outfile
        mpz_set(n, prevn);
        mpz_set(d, prevd);
        rsa_decrypt(m, c, d, n); //decrypt the cipher test
        //gmp_printf("decrypt - %Zd\n",m);
        mpz_export(block, &j, 1, sizeof(uint8_t), 1, 0, m); //export the values in m to the block
        //printf("%s", block + 1);
        fwrite(block + 1, sizeof(uint8_t), j - 1, outfile); //print the block
    }
    //printf("\n%d",x);
    free(block);
    mpz_clears(m, c, prevd, prevn, NULL);
    return;
}

void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) { //signs the value s by calculating th power mod
    pow_mod(s, m, d, n);
}
bool rsa_verify(mpz_t m, mpz_t s, mpz_t e,
    mpz_t n) { //verifies the signiture. if they are the same, then true, if not then false
    mpz_t t;
    mpz_init(t);
    pow_mod(t, s, e, n);
    if (mpz_cmp(t, m) == 0) {
        mpz_clear(t);
        return true;
    }
    mpz_clear(t);
    return false;
}
/*int main(void){
	randstate_init(22234123);
	mpz_t d,p,q,n,e,a,b,tempe,tp,tq;
	mpz_inits(d,p,q,n,e,a,b,tempe,tp,tq,NULL);
	rsa_make_pub(p,q,n,e,256,50);
	gmp_printf("p   %Zd\n",p);
	gmp_printf("q   %Zd\n",q);
	gmp_printf("n   %Zd\n",n);
	gmp_printf("e   %Zd\n",e);
	rsa_make_priv(d,e,p,q);
	gmp_printf("d    %Zd\n",d);
	FILE *infile = fopen("infile.txt","r+");
	FILE *outfile = fopen("outfile.txt","w+");
	FILE *intex = fopen("intex","r+");
	FILE *pvfile = fopen("rsa.priv","w+");
	rsa_encrypt_file(infile,outfile,n,e);
	rsa_write_priv(n,d,pvfile);
	fseek(outfile,0,SEEK_SET );
	rsa_decrypt_file(outfile,intex,n,d);
	mpz_clears(d,p,q,n,e,a,b,tempe,tp,tq,NULL);
	randstate_clear();
	fclose(infile);
	fclose(outfile);
	fclose(intex);
	fclose(pvfile);
}*/
