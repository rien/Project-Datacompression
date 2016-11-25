/*
 * Author:
 * Rien Maertens
 * Rien.Maertens@UGent.be
 * 3de Bachelor Informatica
 * UGent
 *
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "test_vli.h"
#include "../../src/specifiek/variable_length_integer.h"
#include "../test_common/testmacro.h"

uint64_t test_values[63];

void init_test_values(){
    for (size_t i = 0; i < 63; ++i) {
        test_values[i] = (1UL << i);
    }
}

void test_non_zero_bytes(){
    for (size_t i = 0; i < 63; ++i) {
        size_t nzb = non_zero_bytes(&test_values[i]);
        size_t expected =  (i/8)+1;
        if(nzb != expected){
            test_assert("Non zero bytes", false);
            printf("NZB: %lu, Expected: %lu\n", nzb, expected);
        };
    }
}

void test_vli_encode(){
    byte vli[9];
    size_t resulting_bytes;
    for (size_t i = 0; i < 63; ++i) {
        for (size_t j = 0; j < 9; ++j) {
            vli[j] = 0;
        }
        vli_encode((byte*)&test_values[i], 1, (byte*)vli, &resulting_bytes);
        test_assert("Lower 3 bytes (amount of bits coded)", (vli[0] & 0b111) == non_zero_bytes(&test_values[i])-1);
        test_assert("Lower 61 bits", (test_values[i] << 3) == (*((uint64_t*)vli) & (~((uint64_t)0b111))));
        test_assert("Upper 3 bits", (test_values[i] >> 61)  == vli[8]);
    }
}

void test_vli_decode(){
    byte vli[9];
    uint64_t result;
    size_t resulting_bytes;
    for (size_t i = 0; i < 63; ++i) {
        for (size_t j = 0; j < 9; ++j) {
            vli[j] = 0;
        }
        vli_encode((byte*)&test_values[i], 1, vli, &resulting_bytes);
        vli_decode(vli, (byte*)&result, resulting_bytes, NULL);
        test_assert("Decoding went wrong", result == test_values[i]);
        vli[0]++;
    }
}

void test_vli(){
    init_test_values();
    test_non_zero_bytes();
    test_vli_encode();
    test_vli_decode();
}