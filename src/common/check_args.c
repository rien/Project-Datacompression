//
// Created by rien on 10/6/16.
//

#include <stdlib.h>
#include <string.h>
#include "check_args.h"

extern arguments args;

void graceful_exit_printf(const char *msg,...) {
    printf(msg); //expand varargs
    printf("Usage: %s {-d|-c} source destination\n", program_name);
    // close files, doesn't matter if they are open or not
    fclose(args.source);
    fclose(args.destination);
    exit(1);
}

void parse_arguments(arguments* args, int argc, char **argv) {

    // first argument is the name of the executable
    if(argc != (ARG_COUNT + 1)) {
        graceful_exit_printf("Expected %i arguments, but got %i\n", ARG_COUNT, argc - 1);
    } else if (strcmp(argv[1],"-c") == 0){
        args->option = COMPRESS;
    } else if (strcmp(argv[1], "-d") == 0){
        args->option = DECOMPRESS;
    } else {
        graceful_exit_printf("Argument %s not recognised\n", argv[1]);
    }

    // open files and check if everything is ok
    args->source = fopen(argv[2],"r");
    if(args->source == NULL){
        graceful_exit_printf("Could not open file for reading: %s", args->source);
    }
    args->destination = fopen(argv[3], "w");
    if(args->destination == NULL){
        graceful_exit_printf("Could not open file for writing: %s", args->destination);
    }
}
