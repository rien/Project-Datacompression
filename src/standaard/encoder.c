/*
 * Author:
 * Rien Maertens
 * Rien.Maertens@UGent.be
 * 3de Bachelor Informatica
 * UGent
 *
 */

#include <memory.h>
#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include "../common/encoder.h"
#include "../common/common.h"
#include "../common/huffman.h"
#include "../common/file_info.h"
#include "../common/check_args.h"

/**
 * Encode a file with the standard compression algorithm (using huffman).
 *
 * Code for using the burrows-wheeler transformation has been commented.
 *
 * File header
 *
 * - 10 bytes: file signature
 *      -> 'DA3ZIP-BWT' if the burrows wheeler transformation with move-to-front was applied
 *      -> 'DA3ZIP-HUF' if only huffman encoding was used
 *
 * Block header
 *
 * - (if bwt: 2 bytes: starting index of the bwt)
 * - 2 bytes: amount of encoded bytes (<= block size)
 * - 2 bytes: length of the current block
 *
 * Data:
 * - huffman tree
 * - huffman encoded data
 *
 */
void encode(arguments *args) {
    clock_t start_time = clock();                               // start timing

    //char* sig = args->bw_transform ? "DA3ZIP-BWT" : "DA3ZIP-HUF";        // file sig
    char* sig = "DA3ZIP-HUF";
    fwrite(sig, sizeof(char), FILE_SIG_LENGTH, args->destination);       // signature


    byte buffer1[MAX_BLOCK_SIZE];                              // Two buffers
    byte buffer2[MAX_BLOCK_SIZE];

    size_t a_read;                                              // amount of bytes read
    size_t encoded_length;                                           // amount of bytes to be written
    //size_t t_start;                                             // starting position of the bwt-transformed string
    unsigned long long input_file_size = file_size(args->source);            // file size of the source

    while((a_read = fread(buffer1, sizeof(byte), args->block_size, args->source)) > 0){

        // Show progress
        print_progress(file_position(args->source), input_file_size, start_time, true);

        // Burrows wheeler + move to front
        //if(args->bw_transform){
        //    burrows_wheeler_encode(buffer1, a_read, buffer2, &t_start);
        //    move_to_front_encode(buffer2, buffer1, a_read);
        //    fwrite(&t_start, sizeof(uint16_t), 1, args->destination);
        //}

        // Actual compression
        huffman_encode(buffer1, a_read, buffer2, &encoded_length);

        // Original amount of bytes
        fwrite(&a_read, sizeof(uint16_t), 1, args->destination);

        // Write the amount of encoded bytes as a 16 bit unsigned integer
        fwrite(&encoded_length, sizeof(uint16_t), 1, args->destination);

        // Write encoded data
        fwrite(buffer2, sizeof(byte), encoded_length, args->destination);
    }

    // Error handling
    if(ferror(args->source)){
        graceful_exit_printf(args, false, "An error occurred while reading the input file.\n");
    }
    if(ferror(args->destination)){
        graceful_exit_printf(args, false, "An error occurred while writing to the output file.\n");
    }

    // Show stats
    clock_t stop_time = clock();
    unsigned long long output_file_size = file_size(args->destination);
    print_stats(input_file_size, output_file_size, args, (double) (stop_time - start_time) / CLOCKS_PER_SEC, true);
}
