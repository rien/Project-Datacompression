//
// Created by rien on 10/6/16.
//

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "check_args.h"

/**
 * Exit the program prematurely, but close open files.
 *
 * @param args struct with the src and dest file pointers
 * @param usage_needed if a string needs to be printed with usage info
 * @param msg format string with the info to be printed before exiting. Works like printf with varargs.
 */
void graceful_exit_printf(arguments* args, bool usage_needed, const char *msg,...) {

    //expand varargs
    va_list vararg;
    va_start(vararg, msg);
    vprintf(msg, vararg);
    va_end(vararg);

    printf("Usage: %s {-d|-c} source destination\n", program_name);
    // close files, doesn't matter if they are open or not
    if(args->source){
        fclose(args->source);
    }
    if(args->destination){
        fclose(args->destination);
    }
    exit(1);
}

/**
 * Parse the arguments and fill in default values.
 *
 * @param args struct which will be filled in with the parsed values
 * @param argc argument count
 * @param argv list of arguments
 */
void parse_arguments(arguments* args, int argc, char **argv) {

    //args->bw_transform = true;
    //args->block_size   = (args->bw_transform ? (uint16_t) 8192 : (uint16_t) 32767);
    args->source = NULL;
    args->destination = NULL;
    args->block_size = (uint16_t) 32767;

    // first argument is the name of the executable
    if(argc != (ARG_COUNT + 1)) {
        graceful_exit_printf(args, true, "Expected %i arguments, but got %i\n", ARG_COUNT, argc - 1);
    } else if (strcmp(argv[1],"-c") == 0){
        args->option = COMPRESS;
    } else if (strcmp(argv[1], "-d") == 0){
        args->option = DECOMPRESS;
    } else {
        graceful_exit_printf(args, true, "Argument %s not recognised\n", argv[1]);
    }

    // open files and check if everything is ok
    args->source = fopen(argv[2],"r");
    if(args->source == NULL){
        graceful_exit_printf(args, true, "Could not open file for reading: %s\n", argv[2]);
    }
    args->destination = fopen(argv[3], "w");
    if(args->destination == NULL){
        graceful_exit_printf(args, true, "Could not open file for writing: %s\n", argv[3]);
    }
}
