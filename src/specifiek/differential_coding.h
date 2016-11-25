/*
 * Author:
 * Rien Maertens
 * Rien.Maertens@UGent.be
 * 3de Bachelor Informatica
 * UGent
 *
 */

#include <stddef.h>
#include "../common/common.h"
#include "../common/check_args.h"

#ifndef DA3_PROJECT_DIFFERENTIAL_CODING_H
#define DA3_PROJECT_DIFFERENTIAL_CODING_H


void diff_encode(byte *byte_input, byte *byte_output, size_t a_integers, arguments *args);
void diff_decode(byte *byte_input, byte *byte_output, size_t a_integers, arguments *args);


#endif //DA3_PROJECT_DIFFERENTIAL_CODING_H
