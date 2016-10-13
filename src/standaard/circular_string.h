//
// Created by rien on 10/12/16.
//

#ifndef DA3_PROJECT_CIRCULAR_STRING_H
#define DA3_PROJECT_CIRCULAR_STRING_H

#include <stddef.h>

typedef struct {
    char* base;
    size_t offset; // if offset == length, then the value is END (256)
    size_t length;
} circular_string;

char circular_string_get(const circular_string* cstr, size_t index);

char circular_string_first(const circular_string* cstr);

void circular_string_set_last(circular_string* cstr);

int circular_string_compare(const void* a, const void* b);

#endif //DA3_PROJECT_CIRCULAR_STRING_H
