#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/check_args.h"

char* program_name = "specifiek";

int main(int argc, char* argv[]) {
    arguments args = parse_arguments(argc, argv);
    return 0;
}