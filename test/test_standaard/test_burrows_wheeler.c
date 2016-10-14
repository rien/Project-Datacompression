//
// Created by rien on 10/12/16.
//

#include <memory.h>
#include "test_burrows_wheeler.h"
#include "../../src/standaard/burrows_wheeler.h"
#include "../test_common/testmacro.h"

void test_encode(){
    char* input = "boom|gaat";
    char output[9];
    size_t start_pos;
    burrows_wheeler_encode(input, 9, output, &start_pos);
    test_assert("Start pos not correct", 6);
    test_assert("Encoded message not correct",strcmp(output, "gat|oobam") == 0);
}

void test_burrows_wheeler() {
    test_encode();
}
