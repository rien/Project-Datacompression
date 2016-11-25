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

#ifndef DA3_PROJECT_MOVE_TO_FRONT_H
#define DA3_PROJECT_MOVE_TO_FRONT_H

void move_to_front_encode(byte input[], byte output[], size_t length);

void move_to_front_decode(byte input[], byte output[], size_t length);

#endif //DA3_PROJECT_MOVE_TO_FRONT_H
