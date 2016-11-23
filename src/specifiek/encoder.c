//
// Created by rien on 11/22/16.
//

#include <time.h>
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "../common/check_args.h"
#include "../common/common.h"
#include "../common/file_info.h"
#include "../common/bitcode.h"
#include "differential_coding.h"
#include "variable_length_integer.h"
#include "../common/huffman.h"

#define MAX_NUMBER_LENGTH 19


size_t read_numbers(byte* input, byte* output, size_t input_size, arguments* args, bool* end_reached){
    size_t current_byte = 0;
    size_t integers_coded = 0;
    bitcode next_integer;
    byte last_read;
    bitcode_init(&next_integer);


    while (current_byte < input_size && !(*end_reached)){
        //Skip leading whitespaces
        while(input[current_byte] == ' ' && current_byte < input_size){
            current_byte++;
        }

        // Read digits
        while (isdigit(input[current_byte]) &&
                current_byte < input_size &&
                next_integer.length <= MAX_NUMBER_LENGTH) {

            bitcode_store_byte(input[current_byte++], &next_integer);
        }

        /*
         * End of buffer, read the rest of the number
         */

        if(current_byte < input_size){
            // Read a little bit more
            size_t read_successful;
            while((read_successful = fread(&last_read, sizeof(byte), 1, args->source)) > 0 &&
                    isdigit(last_read) &&
                    next_integer.length <= MAX_NUMBER_LENGTH){

                bitcode_store_byte(last_read, &next_integer);
            }
            if(read_successful == 0){
                bitcode_free(&next_integer);
                graceful_exit_printf(args, "Unexpectedly stopped reading.");
            }

        } else {
            last_read = input[current_byte];
        }

        /*
         * Error handling
         */

        // Closing bracket: we reached the end of the file, ignore the rest
        if(last_read == ']'){
            *end_reached = true;
            printf("Reached ending ']', finishing...");

            // We did not encounter a digit
        } else if(next_integer.length == 0) {
            bitcode_free(&next_integer);
            graceful_exit_printf(args,
                                 "Character '%c' was unexpected, I only accept JSON positive integer arrays.",
                                 last_read);
            // The max value is 2^63, which has 19 digits
        } else if(next_integer.length > MAX_NUMBER_LENGTH){
            bitcode_free(&next_integer);
            graceful_exit_printf(args,
                                 "Encountered a number with more than 19 digits. Max value is 2^63");

            // A number was not followed by a comma, wrong delimiter.
        } else if(last_read != ','){
            bitcode_free(&next_integer);
            graceful_exit_printf(args,
                                 "A number was followed by the character '%c'. "
                                         "Numbers should be delimited by commas (,).",
                                 last_read);
        } else {
            // continue
            assert(input[current_byte] == ',' && 0 < next_integer.length && next_integer.length < 20);
            current_byte++;
        }

        /*
         * Process the next integer
         */

        uint64_t value;
        byte* endbyte = &next_integer.array[next_integer.length/8];

        // Parse to unsigned long long
        value = strtoull((char*)next_integer.array, (char**)&endbyte, 10);
        if(value > 9223372036854775808ULL){
            bitcode_free(&next_integer);
            graceful_exit_printf(args, "Number %llu is greater than max value (2^63)", value);
        }

        // Store the integer in the output
        assert(sizeof(uint64_t)*(integers_coded+1) < args->block_size);
        ((uint64_t *)output)[integers_coded++] = value;

        // empty the bitcode
        bitcode_clear_until(0, &next_integer);

    }
    bitcode_free(&next_integer);
    return integers_coded;
}

/**
 * File header
 *
 * - 10 bytes: file signature
 *      -> 'DA3ZIP-SPC' if the specific compression algorithm was used
 *
 * Block header
 *
 * - 2 bytes: amount of encoded integers
 * - 2 bytes: size of the next encoded data
 *
 * Data:
 * - huffman tree
 * - huffman encoded data
 *
 */
void encode(arguments* args){
    clock_t start_time = clock();                               // start timing

    char* sig = "DA3ZIP-SPC";

    fwrite(sig, sizeof(char), FILE_SIG_LENGTH, args->destination);       // signature

    byte buffer1[MAX_BLOCK_SIZE];                              // Two buffers
    byte buffer2[MAX_BLOCK_SIZE];

    size_t a_read;                                              // amount of bytes read
    size_t a_encoded;                                           // amount of bytes to be written
    size_t a_bytes;                                             // amount of bytes used by the variable length integers
    size_t input_file_size = file_size(args->source);            // file size of the source
    size_t blocks = CEIL_DIVISION(input_file_size, args->block_size);  // blocks to process
    size_t current_block = 0;

    fread(buffer1, sizeof(byte), 1, args->source);
    if(buffer1[0] == '['){
        graceful_exit_printf(args, "Unexpected first character, expected a '['");
    };
    bool end_reached = false;

    while((a_read = fread(buffer1, sizeof(byte), args->block_size, args->source)) > 0 && !end_reached){
        current_block++;

        // Show progress, not 100% correct because read_numbers sometimes reads a little bit more
        printf("%lu%% - %lu/%lu\r\n",(current_block*100)/blocks, current_block, blocks);

        // Read the numbers in the input file
        size_t a_integers = read_numbers(buffer1, buffer2, a_read, args, &end_reached);

        // Calculate the difference between integers
        diff_encode(buffer2, buffer1, a_integers, args);

        // Store as variable-length integers
        vri_encode(buffer1, a_integers, buffer2, &a_bytes);

        // Apply huffman
        huffman_encode(buffer2, a_bytes, buffer1, &a_encoded);

        // Write the amount of encoded integers as a 16 bit unsigned integer
        fwrite(&a_integers, sizeof(uint16_t), 1, args->destination);

        // Write encoded data
        fwrite(buffer2, sizeof(byte), a_encoded, args->destination);
    }


    // Error handling
    if(ferror(args->source)){
        graceful_exit_printf(args, "An error occurred while reading the input file.");
    }
    if(ferror(args->destination)){
        graceful_exit_printf(args, "An error occurred while writing to the output file.");
    }

    // Show stats
    clock_t stop_time = clock();
    size_t output_file_size = file_size(args->destination);
    print_stats(input_file_size, output_file_size, (double) (stop_time - start_time) / CLOCKS_PER_SEC, "compression");
}