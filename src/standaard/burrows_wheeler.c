/*
 * Author:
 * Rien Maertens
 * Rien.Maertens@UGent.be
 * 3de Bachelor Informatica
 * UGent
 *
 */

#include <stdlib.h>
#include <memory.h>
#include "burrows_wheeler.h"
#include "circular_string.h"

/**
 * This burrows-wheeler implementation almost works, but because the extra compression gained by this
 * algorithm is not enough to counter the massive increase in encoding time it is not included in the program.
 *
 * Writing the sorting algorithm by hand should give a decent performance gain.
 *
 * To make it easier to implement this algorithm, circular strings are used (@see circular_string.c).
 */


/**
 * Transform the given text using the burrows-wheeler transformation.
 * The index of the first character will be saved in start_pos.
 */
void burrows_wheeler_encode(byte block[], size_t length, byte output[], size_t* start_pos) {

    circular_string rows[length];

    // Initialize the circular strings
    for (size_t i = 0; i < length; ++i) {
        rows[i].base = block;
        rows[i].length = length;
        rows[i].offset = i;
    }

    // Sort them
    qsort(rows, length, sizeof(circular_string), circular_string_compare);

    // Write the last row to the output
    for (size_t i = 0; i < length; ++i) {
        output[i] = circular_string_last(&rows[i]);
        // because the last character is offset - 1
        if (rows[i].offset == 1) {
            *start_pos = i;
        }
    }
}

/**
 * Comparator method to compare the values of two pointer to a char in the same array
 */
int compare_byte_ptr(const void *a, const void *b){
    return (**(byte**) a) - (**(byte**) b);
}

/**
 * Decode a burrows-wheeler transformed text using the given index of the starting character.
 */
void burrows_wheeler_decode(byte *block, size_t length, byte *output, size_t start_pos){
    byte* sorted[length];

    // Write the pointers to each character
    for (size_t i = 0; i < length; ++i) {
        sorted[i] = &block[i];
    }

    // Sort the pointers(according to their value)
    qsort(sorted, length, sizeof(byte *), compare_byte_ptr);

    // Decode
    size_t next = start_pos;
    for (size_t i = 0; i < length; ++i) {
        output[i] = block[next];
        next = sorted[next] - block; // calculate the offset
    }
}

