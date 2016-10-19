//
// Created by rien on 10/11/16.
//

#include <memory.h>
#include "encoder.h"
#include "../common/constanst.h"
#include "circular_string.h"
#include "burrows_wheeler.h"


int encode() {

    char buffer[BLOCK_SIZE];
    char matrix[BLOCK_SIZE];
    size_t a_read;  // amount of bytes read
    size_t t_start; // starting position of the transformed string
    while((a_read = fread(buffer, BLOCK_SIZE, sizeof(char), args.source)) > 0){
        burrows_wheeler_encode(buffer, a_read, matrix, &t_start);
        //move_to_front_encode();

    }
    if(ferror(args.source)){
        graceful_exit_printf("An error occurred while reading the input file.");
    }
    if(ferror(args.destination)){
        graceful_exit_printf("An error occurred while writing to the output file.");
    }
    return 0;
}
