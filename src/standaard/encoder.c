//
// Created by rien on 10/11/16.
//

#include <memory.h>
#include <time.h>
#include "encoder.h"
#include "../common/constanst.h"
#include "circular_string.h"
#include "burrows_wheeler.h"
#include "move_to_front.h"
#include "huffman.h"
#include "../common/file_info.h"
#include "../common/check_args.h"


int encode() {

    uchar buffer1[BLOCK_SIZE];
    uchar buffer2[BLOCK_SIZE];
    size_t a_read;  // amount of bytes read
    size_t a_encoded;
    size_t t_start; // starting position of the transformed string
    size_t input_file_size = file_size(args.source);
    size_t blocks = (input_file_size-1)/BLOCK_SIZE;
    size_t current_block = 0;
    clock_t start_time = clock();
    while((a_read = fread(buffer1, sizeof(uchar), BLOCK_SIZE, args.source)) > 0){
        if(do_burrows_wheeler){
            burrows_wheeler_encode(buffer1, a_read, buffer2, &t_start);
            move_to_front_encode(buffer2, buffer1, a_read);
        }
        uchar* output = huffman_encode(buffer1, a_read, &a_encoded);
        fwrite(output, sizeof(uchar), a_encoded, args.destination);
        free(output);
        printf("%lu%% - %lu/%lu\r\n",(current_block*100)/blocks, current_block, blocks);
        current_block++;
    }
    clock_t stop_time = clock();
    if(ferror(args.source)){
        graceful_exit_printf("An error occurred while reading the input file.");
    }
    if(ferror(args.destination)){
        graceful_exit_printf("An error occurred while writing to the output file.");
    }
    size_t output_file_size = file_size(args.destination);
    printf("Done. %lu%% compression in %.3f seconds.",
           (output_file_size*100)/input_file_size,
           ((double) (stop_time - start_time)) / CLOCKS_PER_SEC);
    return 0;
}
