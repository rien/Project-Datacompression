//
// Created by rien on 10/11/16.
//

#include <memory.h>
#include <time.h>
#include <limits.h>
#include <assert.h>
#include "encoder.h"
#include "../common/common.h"
#include "circular_string.h"
#include "burrows_wheeler.h"
#include "move_to_front.h"
#include "huffman.h"
#include "../common/file_info.h"
#include "../common/check_args.h"

/**
 * File header
 *
 * - 10 bytes: file signature
 *      -> 'DA3ZIP-BWT' if the burrows wheeler transformation with move-to-front was applied
 *      -> 'DA3ZIP-HUF' if only huffman encoding was used
 * - 4 bytes: amount of blocks
 *    (this restricts the maximum file size to encode to around 35 TB, which should be just enough)
 *
 * Block header
 *
 * - if bwt: 2 bytes: starting index of the bwt
 * - 2 bytes: amount of encoded bytes (<= block size)
 * - 2 bytes: size of the next encoded data (excluding possible bwt starting index)
 *
 * Data:
 * - huffman tree
 * - huffman encoded data
 *
 */
void encode() {
    clock_t start_time = clock();                               // start timing

    char* sig = args.bw_transform ? "DA3ZIP-BWT" : "DA3ZIP-HUF";        // file sig

    fwrite(sig, sizeof(char), FILE_SIG_LENGTH, args.destination);       // signature

    byte buffer1[MAX_BLOCK_SIZE];                              // Two buffers on the stack: faster than on the heap
    byte buffer2[MAX_BLOCK_SIZE];

    size_t a_read;                                              // amount of bytes read
    size_t a_encoded;                                           // amount of bytes to be written
    size_t t_start;                                             // starting position of the bwt-transformed string
    size_t input_file_size = file_size(args.source);            // file size of the source
    size_t blocks = CEIL_DIVISION(input_file_size, args.block_size);  // blocks to process
    size_t current_block = 0;

    fwrite(&blocks, sizeof(uint32_t), 1, args.destination);     // amount of blocks

    while((a_read = fread(buffer1, sizeof(byte), args.block_size, args.source)) > 0){
        current_block++;

        // Show progress
        printf("%lu%% - %lu/%lu\r\n",(current_block*100)/blocks, current_block, blocks);

        // Burrows wheeler + move to front
        if(args.bw_transform){
            burrows_wheeler_encode(buffer1, a_read, buffer2, &t_start);
            move_to_front_encode(buffer2, buffer1, a_read);
            fwrite(&t_start, sizeof(uint16_t), 1, args.destination);
        }

        // Actual compression
        huffman_encode(buffer1, a_read, buffer2, &a_encoded);
        assert(a_encoded < MAX_BLOCK_SIZE);

        // Original amount of bytes
        fwrite(&a_read, sizeof(uint16_t), 1, args.destination);

        // Write the amount of encoded bytes as a 16 bit unsigned integer
        fwrite(&a_encoded, sizeof(uint16_t), 1, args.destination);

        // Write encoded data
        fwrite(buffer2, sizeof(byte), a_encoded, args.destination);
    }

    // Error handling
    if(ferror(args.source)){
        graceful_exit_printf("An error occurred while reading the input file.");
    }
    if(ferror(args.destination)){
        graceful_exit_printf("An error occurred while writing to the output file.");
    }

    // Show off how good we are
    clock_t stop_time = clock();
    size_t output_file_size = file_size(args.destination);
    print_stats(input_file_size, output_file_size, (double) (stop_time - start_time) / CLOCKS_PER_SEC, "compression");
}
