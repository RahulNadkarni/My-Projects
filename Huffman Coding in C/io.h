#pragma once

#include "code.h"
#include <stdbool.h>
#include <stdint.h>

extern uint64_t bytes_read;
extern uint64_t bytes_written;
extern uint64_t total_bytes_read;
int read_bytes(int infile, uint8_t *buf, int nbytes);

int write_bytes(int outfile, uint8_t *buf, int nbytes);

bool read_bit(int infile, uint8_t *bit);

void write_code(int outfile, Code *c);

void flush_codes(int outfile);
