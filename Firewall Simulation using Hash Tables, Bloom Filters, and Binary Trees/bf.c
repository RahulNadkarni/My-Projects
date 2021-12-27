#include "bv.h"
#include "bf.h"
#include <stdbool.h>
#include <stdint.h>
#include "salts.h"
#include <stdlib.h>
#include <stdio.h>
#include "speck.h"
struct BloomFilter {
    uint64_t primary[2];
    uint64_t secondary[2];
    uint64_t tertiary[2];
    BitVector *filter;
};
///////////////////////////////
//Function: BloomFilter *bf_create(uin32_t size)
//Description: Creates a Bloom Filter and assigns the necessary salts
//Parameters: (uint32_t) is an unsigned thirty two bit integer that represents the size of the bloom filter
//Returns: This function returns a newly cretaed bloom filter
//////////////////////////////
BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
    if (bf) {
        bf->primary[0] = SALT_PRIMARY_LO;
        bf->primary[1] = SALT_PRIMARY_HI;
        bf->secondary[0] = SALT_SECONDARY_LO;
        bf->secondary[1] = SALT_SECONDARY_HI;
        bf->tertiary[0] = SALT_TERTIARY_LO;
        bf->tertiary[1] = SALT_TERTIARY_HI;
        bf->filter = bv_create(size);
    }
    return bf;
}
///////////////////////////////
//Function: void bf_delete(BloomFilter **bf)
//Description: Deletes a bloom filter and the bit vector filter
//Parameters: (BloomFilter **bf) is a double pointer to a bloom filter
//Returns: Nothing, however, the bloom filter will be deleted
void bf_delete(BloomFilter **bf) {
    if (*bf && (*bf)->filter) {
        bv_delete(&(*bf)->filter);
        free(*bf);
        *bf = NULL;
    }
    return;
}
////////////////////////////
//Function: uint32_t bf_size(BloomFilter *bf)
//Description: Gets the length of the bloom filter as described in the create function
//Parameters: (BloomFilter *bf) is a pointer to the bloom filter
//Returns: This function returns the length of the bloom filter
///////////////////////////
uint32_t bf_size(BloomFilter *bf) {
    return bv_length(bf->filter);
}
///////////////////////////////
//Function: bf_insert(BloomFilter *bf,char oldspeak)
//Description: This function inserts the word into the bloom filter by setting a bit at the index which the hash function returns
//Parameter One: BloomFilter *bf is a pointer to the bloom filter
//Parameter Two: char *oldspeak is a character pointer(ie string) which represents the oldspeak string in the oldspeak.txt file
//Returns: This function returns nothing because it is a void function. Although it does not return anything, the oldspeak word is hashed using the three salts
//////////////////////////////
void bf_insert(BloomFilter *bf, char *oldspeak) {
    bv_set_bit(bf->filter, hash(bf->primary, oldspeak)
                               % bf_size(bf)); // Index retrieved by hashing into the first salt
    bv_set_bit(bf->filter, hash(bf->secondary, oldspeak)
                               % bf_size(bf)); //Index retrieved by Hashing into the second salt
    bv_set_bit(bf->filter, hash(bf->tertiary, oldspeak)
                               % bf_size(bf)); //Index retrieved by hashing into the third salt
}
/////////////////////////////
//Function: bool bf_probe(BloomFilter *bf, char *oldspeak)
//Description: This function looks through the bloom filter for the oldspeak word. Although the veracity of the bloom filter probe can be questionable, its truth value yields to a high degree of certainty that the oldspeak value is in the bloom filter
//Paramter One: BloomFilter *bf is a pointer to the bloom filter
//Parameter Two: char *oldspeak is a character pointer(ie string) which represents the oldspeak string in the oldspeak.txt file
//Returns: This function returns a boolean if the oldspeak word is in the bloom filter, else it returns false
///////////////////////////
bool bf_probe(BloomFilter *bf, char *oldspeak) {
    if (bv_get_bit(bf->filter, hash(bf->primary, oldspeak) % bf_size(bf))
        == 1) { //Look through the first salt
        return true;
    }
    if (bv_get_bit(bf->filter, hash(bf->secondary, oldspeak) % bf_size(bf))
        == 1) { //Look through the second salt
        return true;
    }
    if (bv_get_bit(bf->filter, hash(bf->tertiary, oldspeak) % bf_size(bf))
        == 1) { //Look through the third salt
        return true;
    }
    return false;
}
////////////////////////////
//Function: uint32_t bf_count(BloomFilter *bf)
//Description: This function counts the number of set bits that are present in the bloom filter
//Parameter: BloomFilter *bf is a pointer to the bloom filter
//Returns: This function returns an unsigned thirty two bit integer that represents the number of set bits in the bloom filter bit vector
//////////////////////////
uint32_t bf_count(BloomFilter *bf) {
    uint32_t sets = 0;
    for (uint32_t i = 0; i < bf_size(bf); i++) {
        if (bv_get_bit(bf->filter, i) == 1) {
            sets += 1;
        }
    }
    return sets;
}
////////////////////////////
//Function: bf_print(BloomFilter *bf)
//Description: This function prints the bloom filter
//Paramter: BloomFilter *bf is a pointer to the bloom filter
//Returns: Nothing, this is a void function.
//////////////////////////
void bf_print(BloomFilter *bf) {
    for (uint32_t i = 0; i < bf_size(bf); i++) {
        printf("%d", bv_get_bit(bf->filter, i));
    }
}
