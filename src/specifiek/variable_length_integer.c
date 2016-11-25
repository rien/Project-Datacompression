//
// Created by rien on 11/23/16.
//

#include <assert.h>
#include "variable_length_integer.h"
#include "../common/bitcode.h"

/**
 * Return the amount of bytes that are not zero.
 * This will be the length (in bytes) of the variable-lenth integer representation.
 * ! The number 0 is coded as having one 'non' zero byte.
 */
byte non_zero_bytes(uint64_t *value){
    byte* bytes = (byte*) value;
    byte i = 8;
    while(i > 0 && bytes[--i] == 0){}
    return i + (byte)1;
}

/**
 * Code an array of 64-bit integres as variable-length integer:
 * The anatomy of a variable length integer is the following:
 * - First 3 bits which tell how many bytes are used (+1, because we do not encode 0 bytes)
 * - Then that amount of bytes (the actual data), up to a maximum of 8 bytes
 */
void vli_encode(byte *input_bytes, size_t a_integers, byte *output, size_t *a_bytes) {
    uint64_t* input = (uint64_t*) input_bytes;
    bitcode bc;
    bitcode_init(&bc);
    for (size_t i = 0; i < a_integers; ++i) {
        byte bytes_needed = non_zero_bytes(&input[i]);

        // Store how many bytes are used
        // 0 is not coded, so we can subtract 1 of the bytes_needed if we increment this number while decoding
        bitcode_store_bit(((bytes_needed-1) & 1) != 0, &bc);
        bitcode_store_bit(((bytes_needed-1) & 2) != 0, &bc);
        bitcode_store_bit(((bytes_needed-1) & 4) != 0, &bc);


        // Represent the 64-bit integer as an array of 8 bytes and write it to the bitcode
        byte* bytes = (byte*) &input[i];
        for (size_t j = 0; j < bytes_needed; ++j) {
            bitcode_store_byte(bytes[j], &bc);
        }

    }

    // write to output
    bitcode_write_all(output, a_bytes, &bc);
    bitcode_free(&bc);
}

/**
 * Decode a list of variable-length integers to 64-bit integers.
 */
void vli_decode(byte *input, byte *output_bytes, size_t input_size, size_t *a_integers) {
    bitcode bc;
    bitcode_from_array(input, input_size, &bc);
    uint64_t* output = (uint64_t*) output_bytes;
    size_t i = 0;
    while(bc.cursor != bc.length){
        byte a_bytes = 0;

        // The output of bitcode_write_all will add trailing 0 bits until the byte is full,
        // so if there are less than 8 bits left we can ignore the rest.
        if(bc.length - bc.cursor < 8){
            break;
        }

        // Decode the amount of bytes
        a_bytes |= bitcode_consume_bit(&bc);
        a_bytes |= bitcode_consume_bit(&bc) << 1;
        a_bytes |= bitcode_consume_bit(&bc) << 2;
        a_bytes += 1;
        assert(bc.length - bc.cursor >= a_bytes*8);

        // Fetch the data
        uint64_t value = 0;
        for (size_t j = 0; j < a_bytes; ++j) {
            uint64_t x = 0UL | bitcode_consume_byte(&bc);
            value |= x << (j*8);
        }
        output[i++] = value;
    }
    if(a_integers){
        *a_integers = i;
    }
}


