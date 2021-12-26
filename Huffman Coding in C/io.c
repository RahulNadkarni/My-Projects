#include "code.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "defines.h"
#include "io.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
uint8_t buffer[BLOCK];
uint64_t bits_read = 0;
uint64_t bits_added = 0;
uint64_t bytes_read = 0; 
uint64_t bytes_written = 0;
uint64_t total_bytes_read = 0;

int read_bytes(int infile, uint8_t *buf, int nbytes) { //a function that reads the bytes
    int count = 0;
    bytes_read = 0;
    while ((count = read(infile, buf + bytes_read, nbytes - bytes_read)) > 0) {
        bytes_read += count;
        total_bytes_read += count;
        if ((int) bytes_read == nbytes) {
            break;
        }
    }
    return count;
}
int write_bytes(
    int outfile, uint8_t *buf, int nbytes) { //a function that writes bytes to an outfile
    int count = 0;
    int written = 0;
    while ((count = write(outfile, buf + written, nbytes - written)) > 0) {
        written += count;
        bytes_written += count;
        if (written == nbytes) { // Max bytes to write
            break;
        }
    }
    return count;
}
bool read_bit(int infile, uint8_t *bit) { //a function that takes reads bits
    int end = -1;
    if (bits_read == 0) { // if i == 0 or i == BLOCK
        read_bytes(infile, buffer, BLOCK);

        // Read end characters if less than block
        if (bytes_read < BLOCK) {
            end = 8 * bytes_read + 1;
        }
    }
    *bit = (buffer[bits_read / 8] >> (bits_read % 8) & 0x1); // Get bit
    bits_read = (bits_read + 1) % (BLOCK * 8); // Circular increment
    if ((int) bits_read == end) {
        return false;
    }
    return true;
}
void write_code(
    int outfile, Code *c) { //A function that writes out the completed code to the outfile
    for (uint32_t i = 0; i < code_size(c); i++) { // Write of code size
        uint8_t bit = (c->bits[i / 8] >> (i % 8) & 0x1); // Get bit
        if (bit == 1) {
            buffer[bits_added / 8] |= (0x1 << (bits_added % 8));
        } else { // Clear bit
            buffer[bits_added / 8] &= ~(0x1 << (bits_added % 8));
        }
        bits_added = (bits_added + 1) % (BLOCK * 8); // Circular increment

        if (bits_added == 0) { // Write if i == 0 or i == BLOCK
            write_bytes(outfile, buffer, BLOCK);
            memset(buffer, 0, sizeof(buffer)); // Clear buffer
        }
    }
    return;
}
void flush_codes(int outfile) {
    if (bits_added > 0) { // Check if there are bits to write
        uint64_t bytes = bits_added % (BLOCK * 8) / 8;
        if (bits_added % 8 > 0) { // Add if not full byte
            bytes++;
        }
        write_bytes(outfile, buffer, bytes);
    }
    return;
}
