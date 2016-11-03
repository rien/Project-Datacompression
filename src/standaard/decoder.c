//
// Created by rien on 10/28/16.
//

#include <time.h>
#include <memory.h>
#include <assert.h>
#include "decoder.h"
#include "../common/constanst.h"
#include "../common/uchar.h"
#include "burrows_wheeler.h"
#include "move_to_front.h"
#include "../common/file_info.h"
#include "huffman.h"

void decode() {
    clock_t start_time = clock();
    bool bwt = false;

    byte buffer1[MAX_BLOCK_SIZE];                              // Two buffers on the stack: faster than on the heap
    byte buffer2[MAX_BLOCK_SIZE];

    char signature[FILE_SIG_LENGTH];

    // Read file signature
    if(fread(&signature, sizeof(char), FILE_SIG_LENGTH, args.source) < FILE_SIG_LENGTH) {
        graceful_exit_printf("Error reading file.");
    }

    // The file was encoded using BWT
    if(strncmp(signature, "DA3ZIP-BWT", FILE_SIG_LENGTH) == 0){
        bwt = true;


    } else if (strncmp(signature, "DA3ZIP-HUF", FILE_SIG_LENGTH) != 0){
        graceful_exit_printf("Wrong file signature. This is not a DA3ZIP file.");
    }

    uint16_t block_size;
    fread(&block_size, sizeof(uint16_t), 1, args.source);

    size_t a_read = 0;
    uint16_t a_encoded = 0;
    uint16_t bwt_index = 0;
    size_t input_file_size = file_size(args.source);
    size_t a_decoded;

    while (1){
        if(feof(args.source)){
            break;
        }

        // Show progress
        printf("Decoding %lu%%", ftell(args.source)*100/input_file_size);


        // Block header
        if(bwt){
            fread(&bwt_index, sizeof(uint16_t), 1, args.source);
        }
        fread(&a_encoded, sizeof(uint16_t), 1, args.source);

        // Read data
        a_read = fread(buffer1, sizeof(byte), a_encoded, args.source);
        if(a_read < a_encoded){
            printf("Stopped reading unexpectedly. Help.");
            break;
        }

        // Burrows-wheeler transform
        if(bwt){
            burrows_wheeler_decode(buffer1, a_encoded, buffer2, bwt_index);
            move_to_front_decode(buffer2, buffer1, a_encoded);
        }

        // Decompression
        huffman_decode(buffer1, a_encoded, buffer2, &a_decoded);

        assert(a_decoded == block_size);

        // Write decoded data
        fwrite(buffer2, sizeof(byte), a_decoded, args.destination);
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
    print_stats(input_file_size, output_file_size, (double) (stop_time - start_time) / CLOCKS_PER_SEC, "inflation;");

}
