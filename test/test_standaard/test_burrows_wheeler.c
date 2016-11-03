//
// Created by rien on 10/12/16.
//

#include <string.h>
#include "test_burrows_wheeler.h"
#include "../../src/standaard/burrows_wheeler.h"
#include "../test_common/testmacro.h"

void test_encode(){
    byte* input = BYTE_PTR("boom|gaat");
    byte output[9];
    size_t start_pos;
    burrows_wheeler_encode(input, 9, output, &start_pos);
    test_assert("Start pos not correct", start_pos == 6);
    test_assert("Encoded message not correct", strncmp((char*)output, "gat|oobam",9) == 0);
}

void test_decode(){
    byte* input = BYTE_PTR("gat|oobam");
    byte output[9];
    burrows_wheeler_decode(input, 9, output, 6);
    test_assert("Decoded message not correct", strncmp((char*)output, "boom|gaat", 9) == 0);
}

void test_burrows_wheeler() {
    test_encode();
    test_decode();
}
