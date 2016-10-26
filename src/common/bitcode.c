//
// Created by rien on 10/22/16.
//

#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include "bitcode.h"
#include "constanst.h"

#define CURRENT_BIT(bc) (bc->length % 8)
#define CURRENT_BYTE(bc) (bc->length / 8)


/**
 * Intialize a new bitcode
 */
void bitcode_init(bitcode *bc) {
    bc->array = calloc(sizeof(uchar), 1);
    bc->total_bytes = 1;
    bc->length = 0;
}

/**
 * Initialize a new bitcode with the same content as src.
 * The array of dest should not be allocated.
 */
void bitcode_copy(bitcode *src, bitcode *dest) {
    dest->total_bytes = src->total_bytes;
    dest->length = src->length;
    dest->array = malloc(sizeof(uchar)*src->total_bytes);
    memcpy(dest->array, src->array, sizeof(uchar)*src->total_bytes);
}

/**
 * If the array needs to expand, double the array size and set all the new bytes to 0.
 */
void grow_array_if_necessary(size_t growth, bitcode *bc){
    size_t new_size = bc->total_bytes;
    while(new_size * 8 - 1 < bc->length + growth) {
        new_size *= 2;
    }
    if(new_size != bc->total_bytes) {
        bc->array = realloc(bc->array, sizeof(uchar)*new_size);
        while(bc->total_bytes < new_size){
            bc->array[bc->total_bytes] = 0;
            bc->total_bytes++;
        }
    }
}

/**
 * Add a bit to the current code
 */
void bitcode_store_bit(bool bit, bitcode *bc) {
    grow_array_if_necessary(1,bc);

    // We know all unset bits are 0, zo we can just OR the byte
    bc->array[CURRENT_BYTE(bc)] |= bit << CURRENT_BIT(bc);
    bc->length++;
}

/**
 * Write all the bits in the current code to an uchar array.
 * This expects the destinatio to be large enough.
 */
void bitcode_write_all(uchar *dest, size_t* bits_written,const bitcode *bc) {
    size_t total = (bc->length-1)/8+1;
    memcpy(dest, bc->array, sizeof(uchar)*total);
    // bits_written can be NULL
    if(bits_written){
        *bits_written = bc->length;
    }
}

/**
 * Free the internal array of a bitcode
 */
void bitcode_free(bitcode *bc) {
    free(bc->array);
    bc->array = NULL;
}

/**
 * Clear the bitcode's upper bits until it has bit_count bits left.
 * The upper bits will be set to 0.
 */
void bitcode_clear_until(size_t bit_count, bitcode *bc) {
    assert(bit_count <= bc->length);
    size_t difference = bc->length - bit_count;
    size_t byte = CURRENT_BYTE(bc);

    // check if the upper most byte is not the destination byte
    // so we can clear whole bytes at once
    if(((bit_count-1) / 8) != byte){
        bc->array[byte] = 0;
        byte--;
        difference -= CURRENT_BIT(bc);
    }

    // clear whole bytes at once until we reach the destination byte
    while(difference > 7){
        bc->array[byte] = 0;
        byte--;
        difference -= 8;
    }

    // we reached the destination byte, clear the upper (unneeded) bits, if needed
    if(difference != 0){
        assert(byte == ((bit_count-1)/8));
        bc->array[byte] &= ~(0xFF << (bit_count % 8));
    }

    bc->length = bit_count;
}

/**
 * Clear just one bit
 */
void bitcode_clear_one(bitcode *bc) {
    assert(bc->length != 0);
    bc->length--;
    bc->array[CURRENT_BYTE(bc)] &= ~(0xFF << (CURRENT_BIT(bc)));
}

/**
 * Store a whole byte (8 bits) at once.
 */
void bitcode_store_byte(uchar byte, bitcode *bc) {
    grow_array_if_necessary(8,bc);
    size_t current_bit = CURRENT_BIT(bc);
    size_t current_byte = CURRENT_BYTE(bc);

    bc->array[current_byte]   |= (byte << current_bit);     // lower part
    bc->array[current_byte+1] |= (byte >> (8-current_bit)); // upper part

    bc->length += 8;
}

/**
 * Append the bitcode of src to dest.
 */
void bitcode_append(const bitcode *src, bitcode *dest) {
    assert(src != dest);
    grow_array_if_necessary(src->length, dest);
    size_t to_copy = NEXT_DIV(src->length,8);
    size_t bits_copied = 0;
    size_t current_byte_src = 0;
    size_t current_byte_dest = CURRENT_BYTE(dest);
    size_t shift_left = CURRENT_BIT(dest);
    size_t shift_right = 8 - shift_left;

    dest->array[current_byte_dest] |= (src->array[current_byte_src] << shift_left);
    current_byte_dest++;
    bits_copied += MIN(shift_right,src->length);
    while(bits_copied + 8 <= to_copy){
        dest->array[current_byte_dest] |= src->array[current_byte_src] >> shift_right;
        current_byte_src++;

        dest->array[current_byte_dest] |= src->array[current_byte_src] << shift_left;
        current_byte_dest++;
        bits_copied += 8;
    }
    if(bits_copied != src->length){
        dest->array[current_byte_dest] |= src->array[current_byte_src] >> (shift_right % 8);
        bits_copied += shift_left;
    }
    assert(bits_copied >= src->length);
    dest->length += src->length;
}




#undef CURRENT_BIT
#undef CURRENT_BYTE