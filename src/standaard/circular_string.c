//
// Created by rien on 10/12/16.
//

#include <stdlib.h>
#include "circular_string.h"
#include "stdio.h"

uchar circular_string_get(const circular_string* cstr, size_t index) {
    size_t position = (cstr->offset + index) % cstr->length;
    return cstr->base[position];
}

uchar circular_string_first(const circular_string *cstr) {
    return circular_string_get(cstr, 0);
}

int circular_string_compare(const void* a, const void* b) {
    circular_string* cstr_a = (circular_string*) a;
    circular_string* cstr_b = (circular_string*) b;
    size_t i = 0;
    int diff;
    do{
        diff = circular_string_get(cstr_a, i) - circular_string_get(cstr_b, i);
        ++i;
    } while (diff == 0);
    return diff;
}

uchar circular_string_last(const circular_string *cstr) {
    return circular_string_get(cstr, cstr->length-1);
}