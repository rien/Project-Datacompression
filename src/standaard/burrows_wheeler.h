//
// Created by rien on 10/12/16.
//

#include <stddef.h>
#include "circular_string.h"
#include "../common/uchar.h"

#ifndef DA3_PROJECT_BURROWS_WHEELER_H
#define DA3_PROJECT_BURROWS_WHEELER_H

/*
 * Transform a block of BLOCK_SIZE characters with the burrows wheeler transformation
 */
void burrows_wheeler_encode(byte block[], size_t length, byte output[], size_t* start_pos);

void burrows_wheeler_decode(byte block[], size_t length, byte output[], size_t start_pos);

#endif //DA3_PROJECT_BURROWS_WHEELER_H
