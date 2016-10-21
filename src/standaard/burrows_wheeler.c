//
// Created by rien on 10/12/16.
//

#include <stdlib.h>
#include "burrows_wheeler.h"
#include "../common/uchar.h"

void burrows_wheeler_encode(uchar block[], size_t length, uchar output[], size_t* start_pos) {

    circular_string t_rows[length];

    for (size_t i = 0; i < length; ++i) {
        t_rows[i].base = block;
        t_rows[i].length = length;
        t_rows[i].offset = i;
    }

    qsort(t_rows, length, sizeof(circular_string), circular_string_compare);

    // write the last row to the output
    for (size_t i = 0; i < length; ++i) {
        output[i] = circular_string_last(&t_rows[i]);
        if (t_rows[i].offset == 0) {
            *start_pos = i;
        }
    }
}

