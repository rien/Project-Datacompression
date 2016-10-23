//
// Created by rien on 10/6/16.
//

#include "../test_common/testmacro.h"
#include "test_circular_string.h"
#include "test_burrows_wheeler.h"
#include "test_move_to_front.h"
#include "test_priorityqueue.h"
#include "test_bitcode.h"
#include "test_huffman.h"

int tests_run = 0;
int tests_failed = 0;

int main(int argc, char **argv) {
    test_circular_string();
    test_burrows_wheeler();
    test_move_to_front();
    test_priorityqueue();
    test_bitcode();
    test_huffman();
    summarize_tests;
}