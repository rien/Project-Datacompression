//
// Created by rien on 10/11/16.
//

#include <memory.h>
#include <time.h>
#include <limits.h>
#include "encoder.h"
#include "../common/constanst.h"
#include "circular_string.h"
#include "burrows_wheeler.h"
#include "move_to_front.h"
#include "huffman.h"
#include "../common/file_info.h"
#include "../common/check_args.h"

void encode() {
    clock_t start_time = clock();                               // start timing

    byte buffer1[MAX_BLOCK_SIZE];                              // Two buffers on the stack: faster than on the heap
    byte buffer2[MAX_BLOCK_SIZE];

    size_t a_read;                                              // amount of bytes read
    size_t a_encoded;                                           // amount of bytes to be written
    size_t t_start;                                             // starting position of the bwt-transformed string
    size_t input_file_size = file_size(args.source);            // file size of the source
    size_t blocks = ((input_file_size-1)/args.block_size) + 1;  // blocks to process
    size_t current_block = 0;

    while((a_read = fread(buffer1, sizeof(byte), args.block_size, args.source)) > 0){
        current_block++;

        // show progress
        printf("%lu%% - %lu/%lu\r\n",(current_block*100)/blocks, current_block, blocks);

        // burrows wheeler + move to front
        if(args.bw_transform){
            burrows_wheeler_encode(buffer1, a_read, buffer2, &t_start);
            move_to_front_encode(buffer2, buffer1, a_read);
        }

        // actual compression
        huffman_encode(buffer1, a_read, buffer2, &a_encoded);

        // writeout
        fwrite(buffer2, sizeof(byte), a_encoded, args.destination);
    }

    // error handling
    if(ferror(args.source)){
        graceful_exit_printf("An error occurred while reading the input file.");
    }
    if(ferror(args.destination)){
        graceful_exit_printf("An error occurred while writing to the output file.");
    }

    // show off how good we are
    clock_t stop_time = clock();
    size_t output_file_size = file_size(args.destination);
    print_compression_stats(input_file_size, output_file_size, (double)(stop_time - start_time)/CLOCKS_PER_SEC);
}
