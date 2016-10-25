//
// Created by rien on 10/11/16.
//

#include <memory.h>
#include "encoder.h"
#include "../common/constanst.h"
#include "circular_string.h"
#include "burrows_wheeler.h"
#include "move_to_front.h"
#include "huffman.h"


int encode() {

    uchar buffer1[BLOCK_SIZE];
    uchar buffer2[BLOCK_SIZE];
    size_t a_read;  // amount of bytes read
    size_t a_encoded;
    size_t t_start; // starting position of the transformed string
    while((a_read = fread(buffer1, BLOCK_SIZE, sizeof(uchar), args.source)) > 0){
        burrows_wheeler_encode(buffer1, a_read, buffer2, &t_start);
        move_to_front_encode(buffer2, buffer1, a_read);
        uchar* output = huffman_encode(buffer1, a_read, &a_encoded);

    }
    if(ferror(args.source)){
        graceful_exit_printf("An error occurred while reading the input file.");
    }
    if(ferror(args.destination)){
        graceful_exit_printf("An error occurred while writing to the output file.");
    }
    return 0;
}
