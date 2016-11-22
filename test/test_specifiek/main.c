//
// Created by rien on 10/6/16.
//

#include "../test_common/testmacro.h"
#include "../test_common/common_tests.h"

int tests_run = 0;
int tests_failed = 0;

int main(int argc, char **argv) {
    test_common();
    summarize_tests;
}