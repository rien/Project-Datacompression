//
// Created by rien on 10/12/16.
//

#include <stdlib.h>
#include "burrows_wheeler.h"
#include "../common/uchar.h"
#include "circular_string.h"

void burrows_wheeler_encode(uchar block[], size_t length, uchar output[], size_t* start_pos) {

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
        if (rows[i].offset == 0) {
            *start_pos = i;
        }
    }
}

int sort_indices(const void* a, const void* b, void* arg){
    size_t index_a = *(size_t*) a;
    size_t index_b = *(size_t*) b;
    uchar* string  =  (uchar*)  arg;
    return string[index_a] - string[index_b];
}

void burrows_wheeler_decode(uchar *block, size_t length, uchar *output, size_t start_pos) {
    circular_string first_row[length];
    for (size_t i = 0; i < length; ++i) {
        first_row[i].length = 1;
        first_row[i].base = output;
        first_row[i].offset = i;
    }



}

