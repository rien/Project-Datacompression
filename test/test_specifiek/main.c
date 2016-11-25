/*
 * Author:
 * Rien Maertens
 * Rien.Maertens@UGent.be
 * 3de Bachelor Informatica
 * UGent
 *
 */

#include "../test_common/testmacro.h"
#include "../test_common/common_tests.h"
#include "test_vli.h"

char* program_name = "test_specifiek";

int tests_run = 0;
int tests_failed = 0;

int main(int argc, char **argv) {
    test_common();
    test_vli();
    summarize_tests;
}