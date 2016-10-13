//
// Created by rien on 10/12/16.
//

#include "test_burrows_wheeler.h"
#include "../../src/standaard/burrows_wheeler.h"
#include "../test_common/testmacro.h"

void test_encode(){
    char* input = "boom|gaat";
    circular_string output[9];
    size_t start_pos;
    burrows_wheeler_encode(input, 9, output, &start_pos);
    test_assert("Start pos not correct", 6);
    test_assert("Character on position 0", circular_string_first(&output[0]) == 'g');
    test_assert("Character on position 1", circular_string_first(&output[1]) == 'a');
    test_assert("Character on position 2", circular_string_first(&output[2]) == 't');
    test_assert("Character on position 3", circular_string_first(&output[3]) == '|');
    test_assert("Character on position 4", circular_string_first(&output[4]) == 'o');
    test_assert("Character on position 6", circular_string_first(&output[5]) == 'o');
    test_assert("Character on position 7", circular_string_first(&output[6]) == 'b');
    test_assert("Character on position 8", circular_string_first(&output[7]) == 'a');
    test_assert("Character on position 9", circular_string_first(&output[8]) == 'm');
}

void test_burrows_wheeler() {
    test_encode();
}
