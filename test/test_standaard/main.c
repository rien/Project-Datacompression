/*
 * Author:
 * Rien Maertens
 * Rien.Maertens@UGent.be
 * 3de Bachelor Informatica
 * UGent
 *
 */

#include "../test_common/testmacro.h"
#include "test_circular_string.h"
#include "test_burrows_wheeler.h"
#include "test_move_to_front.h"
#include "../test_common/test_priorityqueue.h"
#include "../test_common/test_bitcode.h"
#include "../test_common/test_huffman.h"
#include "../test_common/common_tests.h"

char* program_name = "test_standaard";

int tests_run = 0;
int tests_failed = 0;

int main(int argc, char **argv) {
    test_common();
    test_circular_string();
    test_burrows_wheeler();
    test_move_to_front();
    test_priorityqueue();
    test_huffman();
    summarize_tests;
}