//
// Created by rien on 10/14/16.
//

#ifndef DA3_PROJECT_UCHAR_H
#define DA3_PROJECT_UCHAR_H

/*
 * Typing uchar is shorten.
 * This is used in places where the sign of character is of importance.
 * (The C standard does not say whether char must be signed or unsigned)
 */

typedef unsigned char uchar;

#define UCHAR(str) (uchar)(str)

#endif //DA3_PROJECT_UCHAR_H
