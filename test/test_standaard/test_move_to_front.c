//
// Created by rien on 10/13/16.
//

#include <memory.h>
#include <malloc.h>
#include "test_move_to_front.h"
#include "../../src/standaard/move_to_front.h"
#include "../test_common/testmacro.h"

void test_move_to_front(){
    byte* test =  BYTE_PTR("Dit is een lange tekst die successvol moet omgezet worden.");
    size_t length = strlen((char*)test);
    byte* buffer = calloc(length+1, sizeof(byte));
    byte* result = calloc(length+1, sizeof(byte));
    move_to_front_encode(test, buffer, length);
    move_to_front_decode(buffer, result, length);
    test_assert("Move to front does not decode or encode correctly", strcmp((char*) test, (char*) result) == 0);
    free(buffer);
    free(result);

    byte* test2 = BYTE_PTR("Deze\0\125string bevat\0 \255rare karakters!");
    size_t length2 = 36;
    buffer = calloc(length2+1, sizeof(byte));
    result = calloc(length2+1, sizeof(byte));
    move_to_front_encode(test2, buffer, length2);
    move_to_front_decode(buffer, result, length2);
    for (size_t i = 0; i < length2; ++i) {
        test_assert("Move to front with weird chars",test2[i] == result[i]);
    }
    free(buffer);
    free(result);
}
