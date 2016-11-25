//
// Created by rien on 10/26/16.
//

#include <malloc.h>
#include <stdio.h>
#include "file_info.h"

/**
 * The current position in a file.
 * Sadly, large file sizes are not measurable with the C99 standard.
 */
unsigned long long int file_position(FILE *file) {
    return (unsigned long long int) ftello(file);
}

/**
 * The size in bytes of a file.
 * Sadly, large file sizes are not measurable with the C99 standard.
 */
unsigned long long file_size(FILE *file) {
    rewind(file);
    fseeko(file, 0L, SEEK_END);
    unsigned  long long size = ftello(file);
    rewind(file);
    return size;
}

const char* file_sizes[] = {"B", "KiB", "MiB", "GiB", "TiB", "PiB"};

/**
 * Print the human readable size of a give amount of bytes.
 */
char *human_readable_size(size_t size) {
    // number = 6 chars
    // space  = 1 char
    // suffix = 2 chars
    // null   = 1 char
    //       + ---
    //          8 chars
    char* resultstr = calloc(10, sizeof(char));
    size_t i = 0;
    double floatsize = (double) size;
    while(floatsize > 1024){
        i++;
        floatsize /= 1024;
    }
    sprintf(resultstr, "%.2f %s",floatsize, file_sizes[i]);
    return resultstr;
}

void print_stats(unsigned long long int src_size, unsigned long long int dest_size, double time, char *action) {
    char* human_readable_src = human_readable_size(src_size);
    char* human_readable_dest = human_readable_size(dest_size);
    char* human_readable_speed = human_readable_size((size_t)(src_size/time));


    printf("Done: %s => %s\n", human_readable_src, human_readable_dest);
    printf("%.2f%% %s in %.2f seconds. %s/s\n",
           ((double)dest_size*100)/(double)src_size,
            action,
            time,
            human_readable_speed);
    printf("Compression rate: %.2f\n", (double)src_size/(double)dest_size);

    free(human_readable_speed);
    free(human_readable_src);
    free(human_readable_dest);
}

