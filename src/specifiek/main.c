#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/check_args.h"

char* program_name = "specifiek";

arguments args;

int main(int argc, char* argv[]) {
    args = parse_arguments(argc, argv);
    return 0;
}