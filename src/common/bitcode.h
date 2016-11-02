//
// Created by rien on 10/22/16.
//

#ifndef DA3_PROJECT_BITCODE_H
#define DA3_PROJECT_BITCODE_H

#include <stddef.h>
#include <stdbool.h>
#include "uchar.h"

typedef struct {
    // array to store the bits
    byte* array;

    // total bytes reserved in the array
    size_t total_bytes;

    // length of the current bitcode, always points to a 0
    size_t length;
} bitcode;

void bitcode_init(bitcode* bc);

void bitcode_copy(bitcode* src, bitcode* dest);

void bitcode_free(bitcode* bc);

void bitcode_store_bit(bool bit, bitcode* bc);

void bitcode_store_byte(byte byte, bitcode* bc);

void bitcode_append(const bitcode* src, bitcode* dest);

void bitcode_write_all(byte *dest, size_t* bytes_written,const bitcode *bc);

void bitcode_clear_until(size_t bit_count, bitcode* bc);

void bitcode_clear_one(bitcode* bc);


#endif //DA3_PROJECT_BITCODE_H
