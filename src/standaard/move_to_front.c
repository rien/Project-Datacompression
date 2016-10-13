//
// Created by rien on 10/13/16.
//

#include <memory.h>
#include "move_to_front.h"

#define TABLE_SIZE 255

/*
 * Return the index of character c in the table and move it to the front (adjust its index to 0)
 */
char move_to_front(char c, char table[]){
    char index = table[c];
    for (size_t i = 0; i < TABLE_SIZE; ++i) {
        if(table[i] < index){
            table[i]++;
        }
    }
    table[c] = 0;
    return index;
}

void init_table(char table[]){
    for (size_t i = 0; i < TABLE_SIZE; ++i) {
        table[i] = (char)i;
    }
}


void move_to_front_encode(char *input, char *output, size_t length) {
    char table[TABLE_SIZE];
    init_table(table);
    for (size_t i = 0; i < length; ++i ) {
        output[i] = move_to_front(input[i], table);
    }
}

void move_to_front_decode(char *input, char *output, size_t length) {
    char table[TABLE_SIZE];
    init_table(table);
    for (size_t i = 0; i < length; ++i) {
        char index = input[i];
        char c = table[index];
        // copying memory with memmove can overlap
        memmove(&table[0], &table[1], (size_t)index);
        table[0] = c;
        output[i] = c;
    }
}


