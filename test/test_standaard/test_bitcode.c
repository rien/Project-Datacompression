//
// Created by rien on 10/22/16.
//

#include "test_bitcode.h"
#include "../../src/common/bitcode.h"
#include "../test_common/testmacro.h"

void test_store(){
    bitcode bc;
    bitcode_init(&bc);
    test_assert("Should be empty", bc.next_bit == 0);
    test_assert("Should have one byte", bc.total_bytes == 1);
    test_assert("Should have an array", bc.array != NULL);

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

    test_assert("Should have written 16 bits", bc.next_bit == 16);
    test_assert("Should be expanded", bc.total_bytes == 4);
    test_assert("First byte should be 0xF0", bc.array[0] == 0xF0);
    test_assert("Second byte should be 0x0F", bc.array[1] == 0x0F);

    bitcode_store_bit(true, &bc);
    bitcode_store_bit(false, &bc);
    bitcode_store_bit(true, &bc);

    test_assert("Should have written 19 bits", bc.next_bit == 19);
    test_assert("Third byte", bc.array[2] == 0b101);

    uchar test[3];
    size_t written;
    bitcode_write_all(test, &written, &bc);

    test_assert("Should have written 19 bits to array", written == 19);
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
    test_assert("Should have written a byte", bc.next_bit == 8);
    test_assert("Test first byte", bc.array[0] == 0xFF);
    test_assert("Test second byte", bc.array[1] == 0);

    bitcode_clear_until(0, &bc);
    bitcode_store_bit(false, &bc);
    bitcode_store_byte(0xFF, &bc);
    test_assert("Should have written 9 bits", bc.next_bit == 9);
    test_assert("Test first byte", bc.array[0] == 0b11111110);
    test_assert("Test second byte", bc.array[1] == 0b1);

    bitcode_clear_until(1, &bc);
    bitcode_store_bit(false, &bc);
    bitcode_store_byte(0xFF, &bc);
    test_assert("Should have written 9 bits", bc.next_bit == 10);
    test_assert("Test first byte", bc.array[0] == 0b11111100);
    test_assert("Test second byte", bc.array[1] == 0b11);

    bitcode_clear_until(2, &bc);
    bitcode_store_bit(false, &bc);
    bitcode_store_bit(false, &bc);
    bitcode_store_bit(false, &bc);
    bitcode_store_bit(false, &bc);
    bitcode_store_bit(false, &bc);
    test_assert("Should have written 7 bits", bc.next_bit == 7);

    bitcode_store_byte(0xFF, &bc);
    test_assert("Test first byte", bc.array[0] == 0b10000000);
    test_assert("Test second byte", bc.array[1] == 0b01111111);
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
    test_assert("Next should be 22", bc.next_bit == 22);

    bitcode_clear_until(18,&bc);
    test_assert("Next should be 18", bc.next_bit == 18);
    test_assert("Only 2 bits hould be set", bc.array[2] == 0b00000011);

    bitcode_clear_until(16, &bc);
    test_assert("Byte should be 0", bc.array[2] == 0);

    bitcode_store_bit(true, &bc);
    bitcode_store_bit(true, &bc);
    test_assert("We should be at 18", bc.next_bit == 18);

    bitcode_clear_until(8, &bc);
    test_assert("We should be at 8", bc.next_bit == 8);
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
    test_assert("We should be at 19", bc.next_bit == 19);

    bitcode_clear_until(3, &bc);
    test_assert("We should be at 3", bc.next_bit == 3);
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
    test_assert("Next bit", bc.next_bit == 10);
    
    bitcode_clear_one(&bc);
    test_assert("Next bit", bc.next_bit == 9);
    test_assert("Data", bc.array[1] == 0b01);

    bitcode_clear_one(&bc);
    test_assert("Next bit", bc.next_bit == 8);
    test_assert("Data", bc.array[1] == 0 && bc.array[0] == 0xFF);

    bitcode_clear_one(&bc);
    test_assert("Next bit", bc.next_bit == 7);
    test_assert("Data", bc.array[1] == 0 && bc.array[0] == 0b01111111);

    bitcode_clear_one(&bc);
    test_assert("Next bit", bc.next_bit == 6);
    test_assert("Data", bc.array[1] == 0 && bc.array[0] == 0b00111111);

    bitcode_clear_one(&bc);
    bitcode_clear_one(&bc);
    bitcode_clear_one(&bc);
    bitcode_clear_one(&bc);
    bitcode_clear_one(&bc);
    bitcode_clear_one(&bc);
    test_assert("Next bit", bc.next_bit == 0);
    test_assert("Data", bc.array[1] == 0 && bc.array[0] == 0);
}



void test_bitcode(){
    test_store();
    test_store_byte();
    test_clear();
    test_clear_one();
}
