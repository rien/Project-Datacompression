//
// Created by rien on 10/28/16.
//

#include <time.h>
#include <memory.h>
#include <assert.h>
#include "../common/decoder.h"
#include "../common/common.h"
#include "burrows_wheeler.h"
#include "move_to_front.h"
#include "../common/file_info.h"
#include "huffman.h"
#include "../common/check_args.h"

void decode(arguments* args) {
    clock_t start_time = clock();
    // bool bwt = false;

    byte buffer1[MAX_BLOCK_SIZE];                              // Two buffers on the stack: faster than on the heap
    byte buffer2[MAX_BLOCK_SIZE];

    size_t a_read = 0;
    uint16_t a_encoded = 0;
    uint16_t encoded_length = 0;
    //uint16_t bwt_index = 0;
    size_t input_file_size = file_size(args->source);
    size_t a_decoded;

    char signature[FILE_SIG_LENGTH];

    // Read file signature
    if(fread(&signature, sizeof(char), FILE_SIG_LENGTH, args->source) < FILE_SIG_LENGTH) {
        graceful_exit_printf(args, "Error reading file.");
    }

    // The file was encoded using only Huffman
    if(strncmp(signature, "DA3ZIP-HUFF", FILE_SIG_LENGTH) == 0) {
    //    bwt = false;

    // The file was encoded using BWT + Huffman
    //} else if (strncmp(signature, "DA3ZIP-HUF", FILE_SIG_LENGTH) == 0){
    //    bwt = true;
    } else {
        graceful_exit_printf(args, "Wrong file signature. This is not a DA3ZIP file.");
    }

    size_t a_blocks = 0;
    size_t current_block = 0;

    fread(&a_blocks, sizeof(uint32_t), 1, args->source);

    while (current_block < a_blocks){
        current_block++;

        if(feof(args->source)){
            break;
        }

        // Show progress
        printf("Decoding %lu%%\n", ftell(args->source)*100/input_file_size);


        // Block header
        //if(bwt){
        //    fread(&bwt_index, sizeof(uint16_t), 1, args->source);    // BWT index
        //}
        fread(&a_encoded, sizeof(uint16_t), 1, args->source);        // Amount of encoded bytes
        fread(&encoded_length, sizeof(uint16_t), 1, args->source);   // Length of the current block

        // Read data
        a_read = fread(buffer1, sizeof(byte), encoded_length, args->source);
        if(a_read < encoded_length){
            printf("Stopped reading unexpectedly. Help.\n");
            break;
        }

        // Decompression
        huffman_decode(buffer1, encoded_length, a_encoded, buffer2, &a_decoded);

        assert(a_decoded == a_encoded);

        // Burrows-wheeler transform
        //if(bwt){
        //    move_to_front_decode(buffer2, buffer1, a_encoded);
        //    burrows_wheeler_decode(buffer1, a_encoded, buffer2, bwt_index);
        //}


        // Write decoded data
        fwrite(buffer2, sizeof(byte), a_decoded, args->destination);
    }

    // Error handling
    if(ferror(args->source)){
        graceful_exit_printf(args, "An error occurred while reading the input file.");
    }
    if(ferror(args->destination)){
        graceful_exit_printf(args, "An error occurred while writing to the output file.");
    }

    // Show off how good we are
    clock_t stop_time = clock();
    size_t output_file_size = file_size(args->destination);
    print_stats(input_file_size, output_file_size, (double) (stop_time - start_time) / CLOCKS_PER_SEC, "inflation;");

}
