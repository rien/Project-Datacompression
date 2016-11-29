/*
 * Author:
 * Rien Maertens
 * Rien.Maertens@UGent.be
 * 3de Bachelor Informatica
 * UGent
 *
 */


#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include "check_args.h"

#ifndef DA3_PROJECT_FILE_INFO_H
#define DA3_PROJECT_FILE_INFO_H

unsigned long long int file_position(FILE* file);

unsigned long long int file_size(FILE* file);

char* human_readable_size(size_t size);

void print_progress(unsigned long long int src_pos, unsigned long long int src_size, clock_t start_time, bool compression);

void print_stats(unsigned long long int src_size, unsigned long long int dest_size, arguments *args, double time,
                 bool compression);


#endif //DA3_PROJECT_FILE_INFO_H
