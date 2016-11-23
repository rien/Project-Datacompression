#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "common/check_args.h"
#include "common/common.h"
#include "common/encoder.h"
#include "common/decoder.h"

char* program_name = "standaard";

int main(int argc, char* argv[]) {
    arguments args;
    parse_arguments(&args, argc, argv);

    switch (args.option){
        // Both encode() and decode() access the arguments as external variables
        case COMPRESS: encode(&args);
            break;
        case DECOMPRESS: decode(&args);
            break;
    }
}