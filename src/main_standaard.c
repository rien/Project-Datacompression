/*
 * Author:
 * Rien Maertens
 * Rien.Maertens@UGent.be
 * 3de Bachelor Informatica
 * UGent
 *
 */

#include "common/check_args.h"
#include "common/encoder.h"
#include "common/decoder.h"


char* program_name = "standaard";

int main(int argc, char* argv[]) {
    arguments args;
    parse_arguments(&args, argc, argv);

    switch (args.option){
        case COMPRESS: encode(&args);
            break;
        case DECOMPRESS: decode(&args);
            break;
    }
}