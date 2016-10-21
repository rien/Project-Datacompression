//
// Created by rien on 10/13/16.
//

#include <stddef.h>
#include "../common/uchar.h"

#ifndef DA3_PROJECT_MOVE_TO_FRONT_H
#define DA3_PROJECT_MOVE_TO_FRONT_H

void move_to_front_encode(uchar input[], uchar output[], size_t length);

void move_to_front_decode(uchar input[], uchar output[], size_t length);

#endif //DA3_PROJECT_MOVE_TO_FRONT_H