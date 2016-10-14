//
// Created by rien on 10/13/16.
//

#include <memory.h>
#include <malloc.h>
#include "test_move_to_front.h"
#include "../../src/standaard/move_to_front.h"
#include "../test_common/testmacro.h"

void test_move_to_front(){
    char* test =  "Dit is een lange tekst die successvol moet omgezet worden.";
    size_t length = strlen(test);
    char* buffer = calloc(length+1, sizeof(char));
    char* result = calloc(length+1, sizeof(char));
    move_to_front_encode(test, buffer, length);
    move_to_front_decode(buffer, result, length);
    test_assert("Move to front does not decode or encode correctly", strcmp(test, result) == 0);
    free(buffer);
    free(result);

    char* test2 = "Deze\0\125string bevat\0 \255rare karakters!";
    size_t length2 = 36;
    buffer = calloc(length2+1, sizeof(char));
    result = calloc(length2+1, sizeof(char));
    move_to_front_encode(test2, buffer, length2);
    move_to_front_decode(buffer, result, length2);
    for (size_t i = 0; i < length2; ++i) {
        test_assert("Move to front with weird chars",test2[i] == result[i]);
    }
}
