/*
 * Author:
 * Rien Maertens
 * Rien.Maertens@UGent.be
 * 3de Bachelor Informatica
 * UGent
 *
 */

#include "differential_coding.h"

/**
 * Encoding which saves the difference between two following integers.
 * The first integer will be saved normally.
 */
void diff_encode(byte *byte_input, byte *byte_output, size_t a_integers, arguments *args) {
    uint64_t* input = (uint64_t*) byte_input;
    uint64_t* output = (uint64_t*) byte_output;

    // The normal value of the first integer is saved
    uint64_t last = input[0];
    output[0] = last;

    for (size_t i = 1; i < a_integers; ++i) {
        if (input[i] > last) {

            // Because our input is strictly ascending we don't have to encode 0
            output[i] = (input[i] - last) - 1;
            last = input[i];
        } else {
            graceful_exit_printf(args,
                                 false,
                                 "Input not in strict ascending order. %llu <= %llu\n",
                                 input[i],
                                 last);
        }
    }
}

/**
 * Restore the values of differenctial coded integers.
 */
void diff_decode(byte *byte_input, byte *byte_output, size_t a_integers, arguments *args) {
    uint64_t* input = (uint64_t*) byte_input;
    uint64_t* output = (uint64_t*) byte_output;

    // The first integer is saved normally
    uint64_t last = input[0];
    output[0] = last;

    for (size_t i = 1; i < a_integers; ++i) {

        // We subtracted 1 while encoding, now we add 1
        last = last + input[i] + 1;

        // Something is wrong when our calculated was greater than the MAX VALUE
        if (last <= 9223372036854775808UL) {
            output[i] = last;

        } else {
            graceful_exit_printf(args,
                                 false,
                                 "Last calculated value greater than max value. %llu > 2^63\n",
                                 last);
        }
    }
}
