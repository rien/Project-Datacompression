//
// Created by rien on 10/13/16.
//

#include <memory.h>
#include "move_to_front.h"
#include "../common/uchar.h"
#include "stdint.h"

#define TABLE_SIZE 255

/*
 * Return the index of character c in the table and move it to the front (adjust its index to 0)
 */
uchar move_to_front(uchar c, uchar *table){
    uchar index = table[c];
    for (size_t i = 0; i < TABLE_SIZE; ++i) {
        if(table[i] < index){
            table[i]++;
        }
    }
    table[c] = 0;
    return index;
}

void init_table(uchar *table){
    for (size_t i = 0; i < TABLE_SIZE; ++i) {
        table[i] = (uchar)i;
    }
}


void move_to_front_encode(uchar *input, uchar *output, size_t length) {
    uchar table[TABLE_SIZE];
    init_table(table);
    for (size_t i = 0; i < length; ++i ) {
        output[i] = move_to_front(UCHAR(input[i]), table);
    }
}

void move_to_front_decode(uchar *input, uchar *output, size_t length) {
    uchar table[TABLE_SIZE];
    init_table(table);
    for (size_t i = 0; i < length; ++i) {
        uchar index = UCHAR(input[i]);
        uchar c = table[index];
        // copying memory with memmove can overlap
        memmove(&table[1], &table[0], (size_t)index);
        table[0] = c;
        output[i] = c;
    }
}


