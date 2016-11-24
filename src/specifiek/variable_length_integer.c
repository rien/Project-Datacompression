//
// Created by rien on 11/23/16.
//

#include <assert.h>
#include "variable_length_integer.h"
#include "../common/bitcode.h"

byte non_zero_bytes(uint64_t *value){
    byte* bytes = (byte*) value;
    byte i = 8;
    while(i > 0 && bytes[--i] == 0){}
    return i + (byte)1;
}

void vli_encode(byte *input_bytes, size_t a_integers, byte *output, size_t *a_bytes) {
    uint64_t* input = (uint64_t*) input_bytes;
    bitcode bc;
    bitcode_init(&bc);
    for (size_t i = 0; i < a_integers; ++i) {
        byte bytes_needed = non_zero_bytes(&input[i]);

        // Store how many bytes are used
        // 0 cannot be coded, so we can subtract 1 of the bytes_needed if we increment this number while decoding
        bitcode_store_bit(((bytes_needed-1) & 1) != 0, &bc);
        bitcode_store_bit(((bytes_needed-1) & 2) != 0, &bc);
        bitcode_store_bit(((bytes_needed-1) & 4) != 0, &bc);


        byte* bytes = (byte*) &input[i];

        for (size_t j = 0; j < bytes_needed; ++j) {
            bitcode_store_byte(bytes[j], &bc);
        }

    }

    // write to output
    bitcode_write_all(output, a_bytes, &bc);
    bitcode_free(&bc);
}

void vli_decode(byte *input, byte *output_bytes, size_t input_size, size_t *a_integers) {
    bitcode bc;
    bitcode_from_array(input, input_size, &bc);
    uint64_t* output = (uint64_t*) output_bytes;
    size_t i = 0;
    while(bc.cursor != bc.length){
        byte a_bytes = 0;
        if(bc.length - bc.cursor < 8){
            break;
        }
        a_bytes |= bitcode_consume_bit(&bc);
        a_bytes |= bitcode_consume_bit(&bc) << 1;
        a_bytes |= bitcode_consume_bit(&bc) << 2;
        a_bytes += 1;
        assert(bc.length - bc.cursor >= a_bytes*8);

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


