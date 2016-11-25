//
// Created by rien on 10/22/16.
//

#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include "bitcode.h"
#include "common.h"

#define CURRENT_BIT(bc) (bc->length % 8)
#define CURRENT_BYTE(bc) (bc->length / 8)


/**
 * Intialize a new bitcode.
 */
void bitcode_init(bitcode *bc) {
    bc->array = calloc(sizeof(byte), 8); // Most CPU's are 64 bit anyway
    bc->total_bytes = 8;
    bc->cursor = 0;
    bc->length = 0;
}

/**
 * Initalize a new bitcode with the exact contents of the given array.
 * The given data will be copied.
 */
void bitcode_from_array(const byte *data, size_t length, bitcode *bc) {
    bc->array = malloc(sizeof(byte)* length);
    memcpy(bc->array, data, sizeof(byte)*length);
    bc->total_bytes = length;
    bc->cursor = 0;
    bc->length = length*8;
}

/**
 * Initialize a new bitcode with the same content as src (copied).
 * The array of dest should not be allocated, or it will be lost.
 */
void bitcode_copy(const bitcode *src, bitcode *dest) {
    dest->total_bytes = src->total_bytes;
    dest->length = src->length;
    dest->array = malloc(sizeof(byte)*src->total_bytes);
    memcpy(dest->array, src->array, sizeof(byte)*src->total_bytes);
}

/**
 * If the array needs to expand, double the array size and clear the new bytes.
 */
void grow_array_if_necessary(size_t growth, bitcode *bc){
    size_t new_size = bc->total_bytes;
    while(new_size * 8 - 1 < bc->length + growth) {
        new_size *= 2;
    }
    if(new_size != bc->total_bytes) {
        bc->array = realloc(bc->array, sizeof(byte)*new_size);

        // Clear the new bytes
        while(bc->total_bytes < new_size){
            bc->array[bc->total_bytes] = 0;
            bc->total_bytes++;
        }
    }
}

/**
 * Add a bit to the current code.
 */
void bitcode_store_bit(bool bit, bitcode *bc) {
    grow_array_if_necessary(1,bc);

    // We know all unset bits are 0, zo we can just OR the byte
    bc->array[CURRENT_BYTE(bc)] |= bit << CURRENT_BIT(bc);
    bc->length++;
}

/**
 * Write all the bits in the current code to an uchar array.
 * This expects the destination to be large enough.
 *
 * @param dest              the array where the data will be written to
 * @param bytes_written     pointer to which the amount of written bytes will be stored
 */
void bitcode_write_all(byte *dest, size_t* bytes_written, const bitcode *bc) {
    size_t total = (bc->length-1)/8+1;
    memcpy(dest, bc->array, sizeof(byte)*total);
    // bytes_written can be NULL
    if(bytes_written){
        *bytes_written = total;
    }
}

/**
 * Free the internal array of a bitcode. The bitcode itself will not be freed.
 */
void bitcode_free(bitcode *bc) {
    free(bc->array);
    bc->array = NULL;
}

/**
 * Clear the bitcode's upper bits until it has bit_count bits left.
 * Bits above that will be cleared.
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
void bitcode_store_byte(byte data, bitcode *bc) {
    grow_array_if_necessary(8,bc);
    size_t current_bit = CURRENT_BIT(bc);
    size_t current_byte = CURRENT_BYTE(bc);

    bc->array[current_byte]   |= (data << current_bit);     // lower part
    bc->array[current_byte+1] |= (data >> (8-current_bit)); // upper part

    bc->length += 8;
}

/**
 * Append the bitcode of src to dest.
 */
void bitcode_append(const bitcode *src, bitcode *dest) {
    assert(src != dest);
    grow_array_if_necessary(src->length, dest);
    size_t to_copy = NEXT_DIV(src->length,8);       // amount of bits to copy
    size_t bits_copied = 0;                         // amount of bits already copied
    size_t current_byte_src = 0;                    // next byte to be copied from
    size_t current_byte_dest = CURRENT_BYTE(dest);  // next byte to be copied to
    size_t shift_left = CURRENT_BIT(dest);          // left shift needed to write the lower part of a byte
    size_t shift_right = 8 - shift_left;            // right shift needed to write the upper partof a byte

    // Fill the current destination byte
    dest->array[current_byte_dest] |= (src->array[current_byte_src] << shift_left);
    bits_copied += MIN(shift_right,src->length);
    current_byte_dest++;

    // Clear the next byte
    dest->array[current_byte_dest] = 0;

    // Write byte per byte, first the lower part and then the upper
    while(bits_copied + 8 <= to_copy){
        dest->array[current_byte_dest] |= src->array[current_byte_src] >> shift_right;
        current_byte_src++;

        dest->array[current_byte_dest] |= src->array[current_byte_src] << shift_left;
        current_byte_dest++;

        // Clear the next byte
        dest->array[current_byte_dest] = 0;

        bits_copied += 8;
    }

    // Check if we need to write the upper part one more time
    if(bits_copied != src->length){
        dest->array[current_byte_dest] |= src->array[current_byte_src] >> (shift_right % 8);
        bits_copied += shift_left;
    }
    assert(bits_copied >= src->length);
    dest->length += src->length;
}

/**
 * Read one bit (starting from the first written bit) and increment the cursor.
 * This allows iterating over the bitcode.
 */
bool bitcode_consume_bit(bitcode *bc) {
    assert(bc->cursor < bc->length);
    size_t n = bc->cursor;
    bc->cursor++;
    // check if the bit is set
    return (bool) (0 != (bc->array[n / 8] & (1 << (n % 8))));
}

/**
 * Read one byte (starting from the first written byte) and increment the cursor by 8.
 * This allows iterating over the bitcode.
 */
byte bitcode_consume_byte(bitcode *bc) {
    assert((bc->cursor + 7) < bc->length);
    size_t n = bc->cursor;
    bc->cursor += 8;
    byte result = 0;
    size_t curr_byte = n / 8;
    size_t curr_bit = n % 8;
    result |= bc->array[curr_byte] >> (curr_bit);
    result |= bc->array[curr_byte + 1] << (8 - curr_bit);
    return result;
}

#undef CURRENT_BIT
#undef CURRENT_BYTE