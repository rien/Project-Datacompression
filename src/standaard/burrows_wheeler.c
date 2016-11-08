//
// Created by rien on 10/12/16.
//

#include <stdlib.h>
#include <memory.h>
#include "burrows_wheeler.h"
#include "circular_string.h"

void burrows_wheeler_encode(byte block[], size_t length, byte output[], size_t* start_pos) {

    circular_string rows[length];


    for (size_t i = 0; i < length; ++i) {
        rows[i].base = block;
        rows[i].length = length;
        rows[i].offset = i;
    }

    qsort(rows, length, sizeof(circular_string), circular_string_compare);

    // write the last row to the output
    for (size_t i = 0; i < length; ++i) {
        output[i] = circular_string_last(&rows[i]);
        // because the last character is offset - 1
        if (rows[i].offset == 1) {
            *start_pos = i;
        }
    }
}

int sort_indices(const void* a, const void* b, void* arg){
    size_t index_a = *(size_t*) a;
    size_t index_b = *(size_t*) b;
    byte* string  =  (byte*)  arg;
    return string[index_a] - string[index_b];
}

int compare_byte_ptr(const void *a, const void *b){
    return (**(byte**) a) - (**(byte**) b);
}

void burrows_wheeler_decode(byte *block, size_t length, byte *output, size_t start_pos){
    byte* sorted[length];

    for (size_t i = 0; i < length; ++i) {
        sorted[i] = &block[i];
    }

    qsort(sorted, length, sizeof(byte *), compare_byte_ptr);

    size_t next = start_pos;
    for (size_t i = 0; i < length; ++i) {
        output[i] = block[next];
        next = sorted[next] - block; // calculate the offset
    }
}

