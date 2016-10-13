//
// Created by rien on 10/6/16.
//

#include "../test_common/testmacro.h"
#include "test_circular_string.h"
#include "test_burrows_wheeler.h"
#include "test_move_to_front.h"

int tests_run = 0;
int tests_failed = 0;

int main(int argc, char **argv) {
    test_circular_string();
    test_burrows_wheeler();
    test_move_to_front();
    summarize_tests;
}