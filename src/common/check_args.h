/*
 * Author:
 * Rien Maertens
 * Rien.Maertens@UGent.be
 * 3de Bachelor Informatica
 * UGent
 *
 */

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
    const char* benchmark_file;
} arguments;

void graceful_exit_printf(arguments* args, bool usage_needed, const char *msg,...);

void parse_arguments(arguments* args, int argc, char* argv[]);

#endif //DA3_PROJECT_CHECK_ARGS_H
