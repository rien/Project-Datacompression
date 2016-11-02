//
// Created by rien on 10/12/16.
//

#include <stdlib.h>
#include "burrows_wheeler.h"
#include "../common/uchar.h"
#include "circular_string.h"

void burrows_wheeler_encode(byte block[], size_t length, byte output[], size_t* start_pos) {

    circular_string rows[length];

    circular_string* rows_sorted[length];

    for (size_t i = 0; i < length; ++i) {
        rows[i].base = block;
        rows[i].length = length;
        rows[i].offset = i;
        rows_sorted[i] = &rows[i];
    }

    qsort(rows_sorted, length, sizeof(circular_string*), circular_string_compare);

    // write the last row to the output
    for (size_t i = 0; i < length; ++i) {
        output[i] = circular_string_last(rows_sorted[i]);
        if (rows_sorted[i]->offset == 0) {
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

void burrows_wheeler_decode(byte *block, size_t length, byte *output, size_t start_pos) {
    circular_string first_row[length];
    for (size_t i = 0; i < length; ++i) {
        first_row[i].length = 1;
        first_row[i].base = output;
        first_row[i].offset = i;
    }
    //qsort(first_row, length, sizeof())



}

