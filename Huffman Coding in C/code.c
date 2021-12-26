#include "defines.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "code.h"
#include <assert.h>
#include <inttypes.h>
#define INDEX MAX_CODE_SIZE - 1
#define BITS  8
Code code_init(void) { //create a code on the stack
    Code c;
    c.top = 0; //set the top to zero
    for (uint8_t i = 0; i < MAX_CODE_SIZE; i++) {
        c.bits[i] = 0; //set the bits arrray to zero
    }
    return c;
}

uint32_t code_size(Code *c) { //function returns code size
    return c->top;
}

bool code_empty(Code *c) { //if empty return true
    return c->top == 0;
}

bool code_full(Code *c) { //if full, return true
    return c->top == ALPHABET;
}

bool code_set_bit(Code *c, uint32_t i) { //a function that sets the bit at an index position
    if (i > MAX_CODE_SIZE) { //if out of bounds return false
        return false;
    }
    c->bits[i / BITS] |= (0x1 << i % BITS); //bit shift the value such that the index value is one
    return true; //return true
}
bool code_clr_bit(Code *c, uint32_t i) { //a function that sets the bit the zero
    c->bits[i / BITS] &= ~(0x1 << i % BITS); //bit shift the value such that the value is zero
    return true;
}
bool code_get_bit(Code *c, uint32_t i) { //a function that gets the bit
    return (c->bits[(i) / BITS] >> i % BITS) & 0x1; //bit shift to get the bit
}
bool code_push_bit(Code *c, uint8_t bit) { //a function that pushes a bit
    if (code_full(c)) { //if full return false
        return false;
    }
    if (bit == 1) { //if the bit is one
        code_set_bit(c, c->top); //set the bit
        c->top++; //increase the index
    } else {
        code_clr_bit(c, c->top); //if zero, clear the bit
        c->top++; //increase the index
    }
    return true;
}
bool code_pop_bit(Code *c, uint8_t *bit) { //a function that pops the bit
    if (code_empty(c)) { //if empty
        return false; //return false
    }
    if (code_get_bit(c, c->top - 1)) { //get the bit
        *bit = 1; //set to one
    } else {
        *bit = 0; ///set to zero
    }
    code_clr_bit(c, c->top - 1); //clear the bit
    c->top--; //decrease the value
    return true;
}
void code_print(Code *c) { //a function that prints the code
    for (uint8_t i = 0; i < MAX_CODE_SIZE; i++) {
        printf("%d", c->bits[i]);
    }
}
