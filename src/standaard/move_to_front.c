//
// Created by rien on 10/13/16.
//

#include <memory.h>
#include "move_to_front.h"
#include "stdint.h"

#define TABLE_SIZE 255

/**
 * Return the index of character c in the table and move it to the front (adjust its index to 0)
 */
byte move_to_front(byte c, byte *table){
    byte index = table[c];
    for (size_t i = 0; i < TABLE_SIZE; ++i) {
        if(table[i] < index){
            table[i]++;
        }
    }
    table[c] = 0;
    return index;
}

/**
 * Initialize the ascii table
 */
void init_table(byte *table){
    for (size_t i = 0; i < TABLE_SIZE; ++i) {
        table[i] = (byte)i;
    }
}

/**
 * Decode an array using the move-to-front algoritm.
 */
void move_to_front_encode(byte *input, byte *output, size_t length) {
    byte table[TABLE_SIZE];
    init_table(table);
    for (size_t i = 0; i < length; ++i ) {
        output[i] = move_to_front(BYTE(input[i]), table);
    }
}

/**
 * Decode an mtf-encoded array.
 */
void move_to_front_decode(byte *input, byte *output, size_t length) {
    byte table[TABLE_SIZE];
    init_table(table);
    for (size_t i = 0; i < length; ++i) {
        byte index = BYTE(input[i]);
        byte c = table[index];
        // copying memory with memmove is allowed overlap
        memmove(&table[1], &table[0], (size_t)index);
        table[0] = c;
        output[i] = c;
    }
}


