/*
 * Author:
 * Rien Maertens
 * Rien.Maertens@UGent.be
 * 3de Bachelor Informatica
 * UGent
 *
 */

#include <malloc.h>
#include "test_bitcode.h"
#include "../../src/common/bitcode.h"
#include "testmacro.h"

void test_store(){
    bitcode bc;
    bitcode_init(&bc);
    test_assert("Should be empty", bc.length == 0);
    test_assert("Should have eight bytes", bc.total_bytes == 8);
    test_assert("Should have an array", bc.array != NULL);

    // Swap the array with a smaller one to allow expanding to happen more quickly
    free(bc.array);
    bc.array = calloc(sizeof(byte), 1);
    bc.total_bytes = 1;

    // write 16 bits

    bitcode_store_bit(false, &bc);
    bitcode_store_bit(false, &bc);
    bitcode_store_bit(false, &bc);
    bitcode_store_bit(false, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);

    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(false, &bc);
    bitcode_store_bit(false, &bc);
    bitcode_store_bit(false, &bc);
    bitcode_store_bit(false, &bc);

    test_assert("Should have written 16 bits", bc.length == 16);
    test_assert("Should be expanded", bc.total_bytes == 4);
    test_assert("First byte should be 0xF0", bc.array[0] == 0xF0);
    test_assert("Second byte should be 0x0F", bc.array[1] == 0x0F);

    bitcode_store_bit(true, &bc);
    bitcode_store_bit(false, &bc);
    bitcode_store_bit(true, &bc);

    test_assert("Should have written 19 bits", bc.length == 19);
    test_assert("Third byte", bc.array[2] == 0b101);

    byte test[3];
    size_t written;
    bitcode_write_all(test, &written, &bc);

    test_assert("Should have written 3 bytes to array", written == 3);
    test_assert("First byte of writeout", test[0] == 0xF0);
    test_assert("Second byte of writeout", test[1] == 0x0F);
    test_assert("Third byte of writeout", test[2] == 0b101);

    bitcode_free(&bc);
    test_assert("Should be freed", bc.array == NULL);
}

void test_store_byte(){
    bitcode bc;
    bitcode_init(&bc);

    bitcode_store_byte(0xFF, &bc);
    test_assert("Should have written a byte", bc.length == 8);
    test_assert("Test first byte", bc.array[0] == 0xFF);
    test_assert("Test second byte", bc.array[1] == 0);

    bitcode_clear_until(0, &bc);
    bitcode_store_bit(false, &bc);
    bitcode_store_byte(0xFF, &bc);
    test_assert("Should have written 9 bits", bc.length == 9);
    test_assert("Test first byte", bc.array[0] == 0b11111110);
    test_assert("Test second byte", bc.array[1] == 0b1);

    bitcode_clear_until(1, &bc);
    bitcode_store_bit(false, &bc);
    bitcode_store_byte(0xFF, &bc);
    test_assert("Should have written 9 bits", bc.length == 10);
    test_assert("Test first byte", bc.array[0] == 0b11111100);
    test_assert("Test second byte", bc.array[1] == 0b11);

    bitcode_clear_until(2, &bc);
    bitcode_store_bit(false, &bc);
    bitcode_store_bit(false, &bc);
    bitcode_store_bit(false, &bc);
    bitcode_store_bit(false, &bc);
    bitcode_store_bit(false, &bc);
    test_assert("Should have written 7 bits", bc.length == 7);

    bitcode_store_byte(0xFF, &bc);
    test_assert("Test first byte", bc.array[0] == 0b10000000);
    test_assert("Test second byte", bc.array[1] == 0b01111111);

    bitcode_free(&bc);
}

void test_read(){
    bitcode bc;
    bitcode_init(&bc);

    bitcode_store_byte(0b10101010, &bc);
    bitcode_store_byte(0b10101010, &bc);
    bitcode_store_byte(0b10101010, &bc);
    bitcode_store_byte(0b10101010, &bc);
    bitcode_store_byte(0b10101010, &bc);
    bitcode_store_byte(0b10101010, &bc);
    bitcode_store_byte(0b10101010, &bc);


    test_assert("Read bit", bitcode_consume_bit(&bc) == false);
    test_assert("Read bit", bitcode_consume_bit(&bc) == true);
    test_assert("Read bit", bitcode_consume_bit(&bc) == false);
    test_assert("Read bit", bitcode_consume_bit(&bc) == true);
    test_assert("Read bit", bitcode_consume_bit(&bc) == false);
    test_assert("Read bit", bitcode_consume_bit(&bc) == true);
    test_assert("Read bit", bitcode_consume_bit(&bc) == false);
    test_assert("Read bit", bitcode_consume_bit(&bc) == true);


    test_assert("Read bit", bitcode_consume_bit(&bc) == false);

    test_assert("Read byte", bitcode_consume_byte(&bc) == 0b01010101);
    test_assert("Read bit", bitcode_consume_bit(&bc) == true);
    test_assert("Read byte", bitcode_consume_byte(&bc) == 0b10101010);

    test_assert("Read bit", bitcode_consume_bit(&bc) == false);
    test_assert("Read bit", bitcode_consume_bit(&bc) == true);
    test_assert("Read bit", bitcode_consume_bit(&bc) == false);
    test_assert("Read bit", bitcode_consume_bit(&bc) == true);
    test_assert("Read bit", bitcode_consume_bit(&bc) == false);
    test_assert("Read byte", bitcode_consume_byte(&bc) == 0b01010101);


    test_assert("Read bit", bitcode_consume_bit(&bc) == true);
    test_assert("Read byte", bitcode_consume_byte(&bc) == 0b10101010);

    bitcode_free(&bc);
}

void test_from_array(){
    bitcode bc;
    byte array[] = {0xFF, 0x00, 0xF0, 0x0F};
    bitcode_from_array(array, 4, &bc);

    bitcode_store_bit(true, &bc);
    test_assert("Read byte", bitcode_consume_byte(&bc) == 0xFF);
    test_assert("Read byte", bitcode_consume_byte(&bc) == 0x00);
    test_assert("Read byte", bitcode_consume_byte(&bc) == 0xF0);
    test_assert("Read byte", bitcode_consume_byte(&bc) == 0x0F);
    test_assert("Read bit", bitcode_consume_bit(&bc) == true);

    bitcode_free(&bc);
}

void test_clear(){
    bitcode bc;
    bitcode_init(&bc);

    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc); // 7

    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc); // 15

    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc); // 18
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc); // 21

    test_assert("First two bytes", bc.array[0] == 0xFF && bc.array[1] == 0xFF);
    test_assert("Third byte", bc.array[2] == 0b111111);
    test_assert("Next should be 22", bc.length == 22);

    bitcode_clear_until(18,&bc);
    test_assert("Next should be 18", bc.length == 18);
    test_assert("Only 2 bits hould be set", bc.array[2] == 0b00000011);

    bitcode_clear_until(16, &bc);
    test_assert("Byte should be 0", bc.array[2] == 0);

    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    test_assert("We should be at 18", bc.length == 18);

    bitcode_clear_until(8, &bc);
    test_assert("We should be at 8", bc.length == 8);
    test_assert("Last bytes should be 0", bc.array[1] == 0 && bc.array[2] == 0);

    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc); // 15

    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc); // 18
    test_assert("We should be at 19", bc.length == 19);

    bitcode_clear_until(3, &bc);
    test_assert("We should be at 3", bc.length == 3);
    test_assert("Byte should be 0", bc.array[3] == 0);
    test_assert("Byte should be 0", bc.array[2] == 0);
    test_assert("Byte should be 0", bc.array[1] == 0);
    test_assert("Byte should be 111", bc.array[0] == 0b0000111);

    bitcode_free(&bc);
}

void test_clear_one(){
    bitcode bc;
    bitcode_init(&bc);

    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc); // 7

    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    
    test_assert("First two bytes", bc.array[0] == 0xFF && bc.array[1] == 0b011);
    test_assert("Next bit", bc.length == 10);
    
    bitcode_clear_one(&bc);
    test_assert("Next bit", bc.length == 9);
    test_assert("Data", bc.array[1] == 0b01);

    bitcode_clear_one(&bc);
    test_assert("Next bit", bc.length == 8);
    test_assert("Data", bc.array[1] == 0 && bc.array[0] == 0xFF);

    bitcode_clear_one(&bc);
    test_assert("Next bit", bc.length == 7);
    test_assert("Data", bc.array[1] == 0 && bc.array[0] == 0b01111111);

    bitcode_clear_one(&bc);
    test_assert("Next bit", bc.length == 6);
    test_assert("Data", bc.array[1] == 0 && bc.array[0] == 0b00111111);

    bitcode_clear_one(&bc);
    bitcode_clear_one(&bc);
    bitcode_clear_one(&bc);
    bitcode_clear_one(&bc);
    bitcode_clear_one(&bc);
    bitcode_clear_one(&bc);
    test_assert("Next bit", bc.length == 0);
    test_assert("Data", bc.array[1] == 0 && bc.array[0] == 0);

    bitcode_free(&bc);
}

void test_append(){
    bitcode bc[4];
    for (size_t i = 0; i < 4; ++i) {
        bitcode_init(&bc[i]);
    }

    // bc0: 11111
    bitcode_store_bit(true, &bc[0]);
    bitcode_store_bit(true, &bc[0]);
    bitcode_store_bit(false, &bc[0]);

    bitcode_store_bit(true, &bc[1]);
    bitcode_store_bit(false, &bc[1]);
    bitcode_store_bit(true, &bc[1]);

    // no overflow
    bitcode_append(&bc[1], &bc[0]);
    test_assert("Size should be 6", bc[0].length == 6);
    test_assert("Content", bc[0].array[0] == 0b101011);

    // with overflow
    bitcode_append(&bc[1], &bc[0]);
    test_assert("Size should be 9", bc[0].length == 9);
    test_assert("Content 0", bc[0].array[0] == 0b01101011);
    test_assert("Content 1", bc[0].array[1] == 0b1);

    // more than 8 bits
    bitcode_store_byte(0b01010101, &bc[2]);
    bitcode_store_byte(0b01010101, &bc[2]);
    bitcode_store_byte(0b01010101, &bc[2]);

    bitcode_append(&bc[2], &bc[0]);
    test_assert("Size should be 33", bc[0].length == 33);
    test_assert("Content 0", bc[0].array[0] == 0b01101011);
    test_assert("Content 1", bc[0].array[1] == 0b10101011);
    test_assert("Content 2", bc[0].array[2] == 0b10101010);
    test_assert("Content 3", bc[0].array[3] == 0b10101010);
    test_assert("Content 4", bc[0].array[4] == 0b0);

    // exact 8 bit aligned
    bitcode_store_byte(0b11110000, &bc[3]);
    bitcode_store_byte(0b11110000, &bc[3]);
    bitcode_append(&bc[2], &bc[3]);
    test_assert("Size should be 40", bc[3].length == 40);
    test_assert("Content 0", bc[3].array[0] == 0b11110000);
    test_assert("Content 1", bc[3].array[1] == 0b11110000);
    test_assert("Content 2", bc[3].array[2] == 0b01010101);
    test_assert("Content 3", bc[3].array[3] == 0b01010101);
    test_assert("Content 4", bc[3].array[4] == 0b01010101);

    // clear them again
    for (size_t j = 0; j < 4; ++j) {
        bitcode_clear_until(0, &bc[j]);
    }
    bitcode_store_byte(0xFF, &bc[0]);
    bitcode_store_byte(0xFF, &bc[0]);
    bitcode_store_byte(0xFF, &bc[0]);
    bitcode_clear_until(22, &bc[0]);

    bitcode_store_byte(0, &bc[1]);
    bitcode_store_byte(0, &bc[1]);
    bitcode_clear_until(14,&bc[1]);
    bitcode_store_bit(true, &bc[1]);

    bitcode_append(&bc[1], &bc[0]);
    test_assert("Size should be 37", bc[0].length == 37);
    test_assert("Content 2", bc[0].array[2] == 0b00111111);
    test_assert("Content 3", bc[0].array[3] == 0b00000000);
    test_assert("Content 4", bc[0].array[4] == 0b10000);

    bitcode_store_bit(false, &bc[2]);
    bitcode_store_bit(false, &bc[2]);
    bitcode_store_bit(false, &bc[2]);
    bitcode_store_byte(0xFF, &bc[2]);

    bitcode_append(&bc[2], &bc[0]);
    test_assert("Size should be 48", bc[0].length == 48);
    test_assert("Content 4", bc[0].array[4] == 0b10000);
    test_assert("Content 5", bc[0].array[5] == 0xFF);

    bitcode_store_byte(0,&bc[3]);
    bitcode_store_byte(0xFF,&bc[3]);
    bitcode_clear_until(10, &bc[3]);

    bitcode_append(&bc[2], &bc[3]);
    test_assert("Size should be 23", bc[3].length == 21);
    test_assert("Content 2", bc[3].array[1] == 0b11100011);
    test_assert("Content 3", bc[3].array[2] == 0b11111);

    for (size_t i = 0; i < 4; ++i) {
        bitcode_free(&bc[i]);
    }
}


void test_bitcode(){
    test_store();
    test_store_byte();
    test_read();
    test_from_array();
    test_clear();
    test_clear_one();
    test_append();
}
