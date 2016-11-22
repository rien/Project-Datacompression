//
// Created by rien on 10/12/16.
//

#ifndef DA3_PROJECT_TESTMACRO_H
#define DA3_PROJECT_TESTMACRO_H

// Testing macro's

#include <stdio.h>

#define test_assert(message, test) \
do { \
    tests_run++; \
    if (!(test)){ \
        printf("Test failed: %s %s:%d\n", (message), __FILE__, __LINE__); \
        tests_failed++; \
    } \
} while (0)

#define summarize_tests do { \
    if (tests_failed == 0) { \
        printf("\nALL TESTS PASSED\n\n"); \
    } \
    printf("Summary: %d succeeded, %d failed, %d total", tests_run - tests_failed, tests_failed, tests_run); \
} while (0)

extern int tests_run;
extern int tests_failed;


#endif //DA3_PROJECT_TESTMACRO_H
