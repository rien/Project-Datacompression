//
// Created by rien on 10/14/16.
//

#ifndef DA3_PROJECT_UCHAR_H
#define DA3_PROJECT_UCHAR_H

#include <stdint.h>

/*
 * A simple byte typedef.
 */

typedef uint8_t byte;

#define BYTE(chr) (byte)(chr)
#define BYTE_PTR(str) (byte*)(str)

#endif //DA3_PROJECT_UCHAR_H
