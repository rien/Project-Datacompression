
#include "common/check_args.h"
#include "common/decoder.h"
#include "common/encoder.h"

char* program_name = "specifiek";

int main(int argc, char* argv[]) {
    arguments args;

    parse_arguments(&args, argc, argv);

    switch (args.option){
        case COMPRESS: encode(&args);
            break;
        case DECOMPRESS: decode(&args);
            break;
    }
    return 0;
}