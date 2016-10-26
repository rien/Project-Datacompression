//
// Created by rien on 10/26/16.
//

#include <malloc.h>
#include "file_info.h"



size_t file_size(FILE *file) {
    rewind(file);
    fseek(file, 0L, SEEK_END);
    size_t size = (size_t)ftell(file);
    rewind(file);
    return size;
}

const char* file_sizes[] = {"B", "KB", "MB", "GB", "TB", "PB"};

char *human_readable_size(size_t size) {
    // number = 4 chars
    // space  = 1 char
    // suffix = 2 chars
    // null   = 1 char
    //       + ---
    //          8 chars
    char* resultstr = calloc(8, sizeof(char));
    size_t i = 0;
    while(size < 1024){
        i++;
        size /= 1024;
    }
    sprintf(resultstr, "%lu %s",size, file_sizes[i]);
    return resultstr;
}
