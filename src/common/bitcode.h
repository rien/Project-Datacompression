//
// Created by rien on 10/22/16.
//

#ifndef DA3_PROJECT_BITCODE_H
#define DA3_PROJECT_BITCODE_H

#include <stddef.h>
#include <stdbool.h>
#include "uchar.h"
#include "common.h"

/**
 * This module is used as an abstraction around the low-level bit manipulation that has to be done.
 * It allows easy writing and reading of individual bits, and groups of bits with variable lengths.
 */


typedef struct {
    // array to store the bits
    byte* array;

    // total bytes reserved in the array
    size_t total_bytes;

    // used to iterate over a bitcode
    size_t cursor;

    // length of the current bitcode, always points to a 0
    size_t length;
} bitcode;

void bitcode_init(bitcode* bc);

void bitcode_from_array(const byte *data, size_t length, bitcode *bc);

void bitcode_copy(const bitcode *src, bitcode *dest);

void bitcode_free(bitcode* bc);

void bitcode_store_bit(bool bit, bitcode* bc);

void bitcode_store_byte(byte data, bitcode* bc);

void bitcode_append(const bitcode* src, bitcode* dest);

void bitcode_write_all(byte *dest, size_t* bytes_written,const bitcode *bc);

void bitcode_clear_until(size_t bit_count, bitcode* bc);

void bitcode_clear_one(bitcode* bc);

bool bitcode_consume_bit(bitcode *bc);

byte bitcode_consume_byte(bitcode *bc);


#endif //DA3_PROJECT_BITCODE_H
