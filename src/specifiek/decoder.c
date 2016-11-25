//
// Created by rien on 11/22/16.
//

#include <time.h>
#include <memory.h>
#include "../common/check_args.h"
#include "../common/common.h"
#include "../common/file_info.h"
#include "../common/huffman.h"
#include "variable_length_integer.h"
#include "differential_coding.h"

/**
 * Write the decoded numbers as text to a buffer, separated by commas.
 *
 * @param input_bytes   the decoded number, an array of bytes which will be cast to 64-bit integres
 * @param a_integers    the amount of 64-bit integers
 * @param output        output buffer
 * @param a_decoded     pointer to which the value will be written of the amount of written bytes
 */
void write_numbers(byte *input_bytes, size_t a_integers, byte *output, size_t *a_decoded) {
    uint64_t* input = (uint64_t*) input_bytes;
    size_t bytes_written = 0;
    for (size_t i = 0; i < a_integers; ++i) {
        bytes_written += sprintf((char*) &output[bytes_written], "%llu,",(unsigned long long) input[i]);
    }
    *a_decoded = bytes_written;
}

/**
 *  Decode a file which was encoded using the specific algorithm.
 */
void decode(arguments *args){
    clock_t start_time = clock();

    byte buffer1[MAX_BLOCK_SIZE];                              // Two buffers on the stack: faster than on the heap
    byte buffer2[MAX_BLOCK_SIZE];

    size_t a_read = 0;
    //uint16_t a_encoded = 0;
    uint16_t encoded_length = 0;
    unsigned long long input_file_size = file_size(args->source);
    size_t a_decoded;
    size_t a_integers;

    char signature[FILE_SIG_LENGTH];

    // Read file signature
    if(fread(&signature, sizeof(char), FILE_SIG_LENGTH, args->source) < FILE_SIG_LENGTH) {
        graceful_exit_printf(args, false, "Error reading the input file.\n");
    }

    // Test if the file is encoded with the specific algorithm
    if(strncmp(signature, "DA3ZIP-SPC", FILE_SIG_LENGTH) != 0) {
        graceful_exit_printf(args, false, "Wrong file signature. This is not a DA3ZIP-SPC file.\n");
    }

    unsigned long long bytes_read = file_position(args->source);

    char bracket = '[';
    fwrite(&bracket, sizeof(char), 1, args->destination);

    while (true){

        if(bytes_read == input_file_size){
            break;
        } else {
            // Show progress
            print_progress(bytes_read, input_file_size, start_time, false);
        }

        fread(&a_integers, sizeof(uint16_t), 1, args->source);       // Amount of encoded integers
        fread(&encoded_length, sizeof(uint16_t), 1, args->source);   // Length of the current block

        // Read data
        a_read = fread(buffer1, sizeof(byte), encoded_length, args->source);
        if(a_read < encoded_length){
            printf("Stopped reading unexpectedly.\n");
            break;
        }

        // Decompression
        //huffman_decode(buffer1, encoded_length, a_encoded, buffer2, &a_decoded);

        // Check if the amount of encoded and decoded bytes match
        //if(a_decoded != a_encoded){
        //    graceful_exit_printf(args, "The amount of decoded bytes was not equal to the"
        //            " amount of encoded bytes. Something went wrong.\n");
        //};

        // Variable Length Integers to normal
        size_t a_integers_decoded;
        vli_decode(buffer1, buffer2, encoded_length, &a_integers_decoded);

        // Check if the amount of encoded and decoded integers match
        if(a_integers != a_integers_decoded){
            graceful_exit_printf(args, false, "The amount of decoded integers was not equal to the"
                    " amount of encoded integers. Something went wrong.\n");
        };

        // Differential decoding
        diff_decode(buffer2, buffer1, a_integers, args);

        // Integers to characters
        write_numbers(buffer1, a_integers, buffer2, &a_decoded);

        // Last ',' must be a closing bracket
        bytes_read = file_position(args->source);
        if(bytes_read == input_file_size){
            buffer2[a_decoded-1] = ']';
        }

        // Write decoded data
        fwrite(buffer2, sizeof(byte), a_decoded, args->destination);
    }


    // Error handling
    if(ferror(args->source)){
        graceful_exit_printf(args, false, "An error occurred while reading the input file.\n");
    }
    if(ferror(args->destination)){
        graceful_exit_printf(args, false, "An error occurred while writing to the output file.\n");
    }

    // Show off how good we are
    clock_t stop_time = clock();
    unsigned long long output_file_size = file_size(args->destination);
    print_stats(input_file_size, output_file_size, (double) (stop_time - start_time) / CLOCKS_PER_SEC, false);
}