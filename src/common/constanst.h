//
// Created by rien on 10/11/16.
//

#ifndef DA3_PROJECT_CONSTANST_H
#define DA3_PROJECT_CONSTANST_H

// A position in the block size should be representable by a 16 bit integer
#define MAX_BLOCK_SIZE 65536

#define FILE_SIG_LENGTH 10

#define NEXT_DIV(n,d) (((((n)-1)/(d))+1)*(d))
#define MIN(x,y) (((x) < (y)) ? (x) : (y))

#endif //DA3_PROJECT_CONSTANST_H
