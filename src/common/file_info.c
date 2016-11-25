//
// Created by rien on 10/26/16.
//

#include <malloc.h>
#include <stdio.h>
#include <ecpglib.h>
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

/**
 * Show the current progress we have.
 */
void print_progress(unsigned long long int src_pos, unsigned long long int src_size, clock_t start_time, bool compression) {
    double time = (double)(clock() - start_time)/CLOCKS_PER_SEC;
    double progress_percent = (double)100*src_pos/(double)src_size;

    // Status bar:

    size_t i = 0;
    char bar[24]; // '[' + 20 spaces + ']' + '\0'
    unsigned long long int bar_progress = (20*src_pos)/src_size;
    bar[0] = '[';
    bar[22] = ']';
    bar[23] = '\0';
    while(i < bar_progress){
        bar[i+1] = '=';
        i++;
    }
    bar[i+1] = '>';
    while(i < 20){
        bar[i+2] = ' ';
        i++;
    }

    printf("%s     %s    Progress: %.2f%%    Time: %.2fs\r",
           compression ? "Compressing..." : "Decompressing...",
           bar, progress_percent, time);

    fflush(stdout);
}

/**
 * Print statistics of the current compression/decompression operation
 */
void print_stats(unsigned long long int src_size, unsigned long long int dest_size, double time, bool compression) {
    char* human_readable_src = human_readable_size(src_size);
    char* human_readable_dest = human_readable_size(dest_size);
    char* human_readable_speed = human_readable_size((size_t)(src_size/time));


    printf("Done: %s => %s\n", human_readable_src, human_readable_dest);
    printf("%.2f%% %s in %.2f seconds, %s/s written.\n",
           ((double)dest_size*100)/(double)src_size,
            compression ? "compression" : "decompression",
            time,
            human_readable_speed);
    printf("Ratio: %.2f\n", (double)src_size/(double)dest_size);

    free(human_readable_speed);
    free(human_readable_src);
    free(human_readable_dest);
}

