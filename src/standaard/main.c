#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/check_args.h"
#include "../common/constanst.h"
#include "encoder.h"

char* program_name = "standaard";

arguments args;


int main(int argc, char* argv[]) {
    parse_arguments(&args, argc, argv);

    switch (args.option){
        // Both encode() and decode() access the arguments as external variables
        case COMPRESS: encode();
            break;
        case DECOMPRESS:; //TODO
            break;
    }
}