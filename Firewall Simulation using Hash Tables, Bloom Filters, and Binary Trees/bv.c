#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "bv.h"

struct BitVector {
    uint32_t length;
    uint8_t *vector;
};
////////////////////////////////////
//Funciton: BitVector *bv_create(uint32_t length);
//Description: Creates a BitVector and instantiates the length and the vector array.
//Parameters: (length) is a uint32_t and defines the size for a bit vector
//Returns: This function returns a pointer to a bit vector
///////////////////////////////////
BitVector *bv_create(uint32_t length) {
    BitVector *bv = (BitVector *) malloc(sizeof(BitVector));
    bv->length = length;
    bv->vector = (uint8_t *) calloc(length, sizeof(uint8_t)); //create
    return bv;
}
/////////////////////////////////////////
//Function: void bv_delete(BitVector **bv)
//Description: Deletes a Bit Vector
//Parameters: (BitVector **bv) is a double pointer to a bit vector
//Returns: Nothing, this is a void function.
////////////////////////////////////////
void bv_delete(BitVector **bv) {
    if (*bv && (*bv)->vector) {
        free((*bv)->vector);
        free(*bv);
        *bv = NULL;
    }
}
/////////////////////////////////////
//Funciton: bv_length(BitVector *bv)
//Description: Returns the length of a bit vector as described in the create function
//Parameters: (BitVector *bv) is a pointer to a Bit Vector
//Returns: This function returns the length of the bit vector as is specified in the create function
///////////////////////////////////
uint32_t bv_length(BitVector *bv) {
    return bv->length;
}
//////////////////////////////////
//Function: bv_set_bit(BitVector *bv, uint32_t i)
//Description: Sets a bit by ORing a bit in the bit vector
//Parameter One: (BitVector *bv) takes in a pointer to a bit vector that has been created with bv_create
//Parameter Two: (uint32_t i) is an unsigned thirty two bit integer that represents the index of which bit to set to one
//Returns: This function returns a boolean variable if the operation was successful and false if not
//Citations: The code for the bitwise OR operation was provided by Professor Darrell Long in bv8.h in the CSE13S Resources Repository
///////////////////////////////
bool bv_set_bit(BitVector *bv, uint32_t i) {
    if (i > bv->length) {
        return false;
    }
    bv->vector[i / 8] |= (0x1 << i % 8);
    return true;
}
////////////////////////////////
//Function: bv_clr_bit(BitVector *bv, uint32_t i)
//Description: Clears a bit(ie. sets a bit at the index to zero) by utilizing a bitwise AND
//Parameter One: (BitVector *bv) takes in a pointer to a bit vector that has been created with bv_create
//Parameter Two: (uint32_t i) is an unsigned thirty two bit integer that represents the index of which bit to set to one
//Returns: This function returns a true if the operation was successful if the bit was cleared and false if it was not
//Citations: The code for the bitwise AND operation was provided by Professor Darrell Long in bv8.h in the CSE13S Resources Repository
bool bv_clr_bit(BitVector *bv, uint32_t i) {
    if (i > bv->length) {
        return false;
    }
    bv->vector[i / 8] &= ~(0x1 << i % 8);
    return true;
}
////////////////////////////////
//Function: bv_get_bit(BitVector *bv, uint32_t i)
//Description: Gets a bit from the bit vector given the passed in the index
//Parameter One: (BitVector *bv) takes in a pointer to a bit vector that has been created with bv_create
//Parameter Two: (uint32_t i) is an unsigned thirty two bit integer that represents the index of which bit to set to one
//Returns: This function returns a true if the bit returned is a one and a false if the bit returned is a zero
//Citations:The code for the Right Shift operation was provided by Professor Darrell Long in bv8.h in the CSE13S Resources Repository
bool bv_get_bit(BitVector *bv, uint32_t i) {
    return (bv->vector[(i) / 8] >> i % 8) & 0x1;
}
////////////////////////////////
//Function: bv_print(BitVector *bv)
//Description: Prints out the bit vector
//Parameter One: (BitVector *bv) is a pointer to a bit vector
//Returns: Nothing, this is a simple print function
//////////////////////////////
void bv_print(BitVector *bv) {
    for (uint8_t i = 0; i < bv->length; i++) {
        printf("%d", bv->vector[i]);
    }
}
