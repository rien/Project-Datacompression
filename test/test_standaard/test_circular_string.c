/*
 * Author:
 * Rien Maertens
 * Rien.Maertens@UGent.be
 * 3de Bachelor Informatica
 * UGent
 *
 */

#include <memory.h>
#include <stdlib.h>
#include "test_circular_string.h"
#include "../../src/standaard/circular_string.h"
#include "../test_common/testmacro.h"


circular_string get_c_str(byte* base){
    circular_string c_str;
    c_str.base = base;
    c_str.length = strlen((char*)base);
    c_str.offset = 0;
    return c_str;
}

void test_get(){
    byte* str = BYTE_PTR("Hallo test string.");
    circular_string c_str = get_c_str(str);


    test_assert("First character should be H", circular_string_first(&c_str) == 'H');
    test_assert("Second character should be a", circular_string_get(&c_str, 1) == 'a');
    test_assert("Not circular", circular_string_get(&c_str, c_str.length) == 'H');

    c_str.offset = 6;
    test_assert("First character should be t", circular_string_first(&c_str) == 't');
    test_assert("Second character should be e", circular_string_get(&c_str, 1) == 'e');
    test_assert("Not circular", circular_string_get(&c_str, c_str.length - 6) == 'H');
}

void test_get_last(){
    byte* str = BYTE_PTR("Hallo test string.");
    circular_string c_str = get_c_str(str);

    test_assert("Expected '.'", circular_string_last(&c_str) == '.');
}

void  test_compare() {
    byte* test  = BYTE_PTR("aaabc\0\0\0\0");
    circular_string cstr1 = get_c_str(test);
    cstr1.offset = 1;
    cstr1.length = 9;

    circular_string cstr2 = get_c_str(test);
    cstr2.offset = 2;
    cstr2.length = 9;

    test_assert("test with normal character", circular_string_compare(&cstr1, &cstr2) < 0);

    cstr1.offset = 6; // first '\0'
    cstr2.offset = 5; // second '\0'
    test_assert("test with null chars", circular_string_compare(&cstr1, &cstr2) > 0);

    byte *test2 = BYTE_PTR("bcacd");
    circular_string cstr_arr[5];
    for (size_t i = 0; i < 5; ++i) {
        cstr_arr[i] = get_c_str(test2);
        cstr_arr[i].offset = i;
    }

    qsort(cstr_arr, 4, sizeof(circular_string), circular_string_compare);

    test_assert("Sorted 0 should be 'a'", circular_string_first(&cstr_arr[0]) == 'a');
    test_assert("Sorted 1 should be 'b'", circular_string_first(&cstr_arr[1]) == 'b');
    test_assert("Sorted 2 should be 'c'", circular_string_first(&cstr_arr[2]) == 'c');
    test_assert("Sorted 3 should be 'c'", circular_string_first(&cstr_arr[3]) == 'c');
    test_assert("Sorted 4 should be 'd'", circular_string_first(&cstr_arr[4]) == 'd');

}


void test_circular_string() {
    test_get();
    test_get_last();
    test_compare();
}
