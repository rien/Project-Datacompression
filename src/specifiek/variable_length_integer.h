/*
 * Author:
 * Rien Maertens
 * Rien.Maertens@UGent.be
 * 3de Bachelor Informatica
 * UGent
 *
 */

#ifndef DA3_PROJECT_VARIABLE_LENGTH_INTEGER_H
#define DA3_PROJECT_VARIABLE_LENGTH_INTEGER_H


#include <stddef.h>
#include "../common/common.h"

byte non_zero_bytes(uint64_t *value);
void vli_encode(byte *input_bytes, size_t a_integers, byte *output, size_t *a_bytes);
void vli_decode(byte *input, byte *output_bytes, size_t input_size, size_t *a_integers);

#endif //DA3_PROJECT_VARIABLE_LENGTH_INTEGER_H
