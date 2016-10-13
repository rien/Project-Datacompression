//
// Created by rien on 10/12/16.
//

#include <stdlib.h>
#include "burrows_wheeler.h"

void burrows_wheeler_encode(char block[], size_t length, circular_string t_rows[], size_t* start_pos) {

    for (size_t i = 0; i < length; ++i) {
        t_rows[i].base = block;
        t_rows[i].length = length;
        t_rows[i].offset = i;
    }

    qsort(t_rows, length, sizeof(circular_string), circular_string_compare);

    // set to last row and remember the starting position
    for (size_t i = 0; i < length; ++i) {
        circular_string_set_last(&t_rows[i]);
        if (t_rows[i].offset == 0) {
            *start_pos = i;
        }
    }
}

