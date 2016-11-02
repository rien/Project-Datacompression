//
// Created by rien on 10/6/16.
//

#ifndef DA3_PROJECT_CHECK_ARGS_H
#define DA3_PROJECT_CHECK_ARGS_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define ARG_COUNT 3

extern char* program_name;

typedef enum {
    COMPRESS,
    DECOMPRESS
} option_type;

typedef struct {
    FILE* source;
    FILE* destination;
    option_type option;
    uint16_t block_size;
    bool bw_transform;
} arguments;

void graceful_exit_printf(const char *msg,...);

void parse_arguments(arguments* args, int argc, char* argv[]);

#endif //DA3_PROJECT_CHECK_ARGS_H
