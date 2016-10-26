//
// Created by rien on 10/26/16.
//


#include <stddef.h>
#include <stdio.h>

#ifndef DA3_PROJECT_FILE_INFO_H
#define DA3_PROJECT_FILE_INFO_H


size_t file_size(FILE* file);

char* human_readable_size(size_t size);


#endif //DA3_PROJECT_FILE_INFO_H
