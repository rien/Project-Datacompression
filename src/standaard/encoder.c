//
// Created by rien on 10/11/16.
//

#include <memory.h>t
#include <stdlib.h>
#include "encoder.h"
#include "../common/constanst.h"

// DIRTY HACK: we cannot pass variables to the comp function of qsort
char* block_start;

unsigned short read_char_and_next(char** ptr){
    if(*ptr == NULL){
        // return a high number so the EOF char appears last in the sorted list
        // and reset the pointer to the start of the block buffer
        *ptr = block_start;
        return 256;
    } else {
        // access the pointer value
        unsigned short v = (unsigned short)(*(*ptr));
        // move the pointer to the next in the array
        ++(*ptr);
        // return the character value
        return v;
    }
}

/*
 * Compare the strings where a and b point to lexicographically
 * The strings are 'circular': when a or b is NULL we start reading
 * from the beginning of the block.
 */
int lex_cmp_ptr(const void* a, const void* b){
    char* ap = a;
    char* bp = b;
    unsigned short av;
    unsigned short bv;
    // This loop will finish because the EOF location is always different
    do{
        av = read_char_and_next(&ap);
        bv = read_char_and_next(&bp);
    } while(av == bv);
    return av - bv;
}

/*
 * Transform a block of BLOCK_SIZE characters with the burrows wheeler transformation
 */
void burrows_wheeler(char block[],char* t_rows[], size_t length){

    for (int i = 0; i < length; ++i) {
        t_rows[i] = &block[i];
        memset(t_rows[length], NULL, BLOCK_SIZE - length + 1);
    }
    block_start = block;
    qsort(t_rows, length + 1, sizeof(char*), lex_cmp_ptr);
    // calculate last row ...

}

int encode() {

    char buffer[BLOCK_SIZE];
    char* matrix[BLOCK_SIZE+1]; // +1 for end of field (=NULL)
    size_t a_read;
    while((a_read = fread(buffer, BLOCK_SIZE, sizeof(char), args.source)) > 0){
        burrows_wheeler(buffer, matrix, a_read);

    }
    if(ferror(args.source)){
        graceful_exit_printf("An error occurred while reading the input file.");
    }
    if(ferror(args.destination)){
        graceful_exit_printf("An error occurred while writing to the output file.");
    }
    return 0;
}
