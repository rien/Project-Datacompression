//
// Created by rien on 10/11/16.
//

#ifndef DA3_PROJECT_CONSTANST_H
#define DA3_PROJECT_CONSTANST_H

// A position in the block size should be representable by a 16 bit integer
#include <stdint.h>

#define MAX_BLOCK_SIZE 65536

#define FILE_SIG_LENGTH 10

#define CEIL_DIVISION(n,d)  ((((n) + (d) +1))/(d))
#define NEXT_DIV(n,d)       (((((n) + (d) +1))/(d))*(d))
#define MIN(x,y)            (((x) < (y)) ? (x) : (y))

/*
 * A simple byte typedef.
 */

typedef uint8_t byte;

#define BYTE(chr) (byte)(chr)
#define BYTE_PTR(str) (byte*)(str)

#endif //DA3_PROJECT_CONSTANST_H
