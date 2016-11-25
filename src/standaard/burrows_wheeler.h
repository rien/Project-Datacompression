/*
 * Author:
 * Rien Maertens
 * Rien.Maertens@UGent.be
 * 3de Bachelor Informatica
 * UGent
 *
 */

#include <stddef.h>
#include "circular_string.h"

#ifndef DA3_PROJECT_BURROWS_WHEELER_H
#define DA3_PROJECT_BURROWS_WHEELER_H

/*
 * Transform a block of BLOCK_SIZE characters with the burrows wheeler transformation
 */
void burrows_wheeler_encode(byte block[], size_t length, byte output[], size_t* start_pos);

void burrows_wheeler_decode(byte block[], size_t length, byte output[], size_t start_pos);

#endif //DA3_PROJECT_BURROWS_WHEELER_H
