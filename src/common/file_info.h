//
// Created by rien on 10/26/16.
//


#include <stddef.h>
#include <stdio.h>

#ifndef DA3_PROJECT_FILE_INFO_H
#define DA3_PROJECT_FILE_INFO_H

unsigned long long int file_position(FILE* file);

unsigned long long int file_size(FILE* file);

char* human_readable_size(size_t size);

void print_stats(unsigned long long src_size, unsigned long long dest_size, double time, char *action);


#endif //DA3_PROJECT_FILE_INFO_H
