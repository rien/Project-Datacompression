//
// Created by rien on 10/6/16.
//

#include <stdlib.h>
#include <string.h>
#include "check_args.h"

void graceful_exit(const char *msg, ...) {
    printf(msg);
    printf("Usage: %s {-d|-c} source destination\n", program_name);
    exit(1);
}

arguments parse_arguments(int argc, char **argv) {
    arguments result;

    // first argument is the name of the executable
    if(argc != (ARG_COUNT + 1)) {
        graceful_exit("Expected %i arguments, but got %i\n", ARG_COUNT, argc - 1);
    } else if (strcmp(argv[1],"-c") == 0){
        result.option = COMPRESS;
    } else if (strcmp(argv[1], "-d") == 0){
        result.option = DECOMPRESS;
    } else {
        graceful_exit("Argument %s not recognised\n", argv[1]);
    }

    result.source = fopen(argv[2], "r");
    result.destination = fopen(argv[3], "w");

    // fopen returns NULL on error
    if(!result.source){
        graceful_exit("Could not read file: %s\n", argv[2]);
    }
    if(!result.destination){
        graceful_exit("Could not open file to writing: %s\n", argv[3]);
    }

    return result;
}
