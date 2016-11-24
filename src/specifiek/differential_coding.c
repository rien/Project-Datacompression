//
// Created by rien on 11/23/16.
//

#include "differential_coding.h"

void diff_encode(byte *byte_input, byte *byte_output, size_t a_integers, arguments *args) {
    uint64_t* input = (uint64_t*) byte_input;
    uint64_t* output = (uint64_t*) byte_output;

    // The normal value of the first integer is saved
    uint64_t last = input[0];
    output[0] = last;

    for (size_t i = 1; i < a_integers; ++i) {
        if (input[i] > last) {
            output[i] = (input[i] - last) - 1;
            last = input[i];
        } else {
            graceful_exit_printf(args,
                                 "Input not in strict ascending order. %llu <= %llu\n",
                                 input[i],
                                 last);
        }
    }
}

void diff_decode(byte *byte_input, byte *byte_output, size_t a_integers, arguments *args) {
    uint64_t* input = (uint64_t*) byte_input;
    uint64_t* output = (uint64_t*) byte_output;

    // The first integer is saved normally
    uint64_t last = input[0];
    output[0] = last;

    for (size_t i = 1; i < a_integers; ++i) {
        last = last + input[i] + 1;
        if (last <= 9223372036854775808UL) {
            output[i] = last;

        } else {
            graceful_exit_printf(args,
                                 "Last calculated value greater than max value. %llu > 2^63\n",
                                 last);
        }
    }
}
