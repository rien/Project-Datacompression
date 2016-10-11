//
// Created by rien on 10/11/16.
//

#include <memory.h>
#include <stdlib.h>
#include "encoder.h"
#include "../common/constanst.h"



/*
 * Transform a block of BLOCK_SIZE characters with the burrows wheeler transformation
 */
void burrows_wheeler(char block[],char* t_rows[], size_t length){

    for (int i = 0; i < length; ++i) {
        t_rows[i] = &block[i];
        memset(t_rows[length], NULL, BLOCK_SIZE - length + 1);
    }
    qsort();


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
