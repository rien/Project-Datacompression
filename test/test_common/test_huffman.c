/*
 * Author:
 * Rien Maertens
 * Rien.Maertens@UGent.be
 * 3de Bachelor Informatica
 * UGent
 *
 */

#include <collection_tools.h>
#include "test_huffman.h"
#include "../../src/common/huffman.h"
#include "../../src/common/bitcode.h"

void test_huffman_encode(){
    byte buffer[BLOCK_SIZE];
    byte* input = BYTE_PTR("AAAAAABCCCCCCDDEEEEE");
    size_t input_length = strlen((char*)input);

    huffman_dictionary hd;
    huffman_init(&hd);
    /*
     * The tree should look like this:
     *
     *         20
     *     ----------
     *     |        8
     *     |     -------
     *    12     |     3
     *   -----   |   -----
     *   A   C   E   D   B
     *   6   6   5   2   1
     *
     *  Dictionary code: 001A1C01E01D1B
     */
    huffman_build_tree(input, input_length, &hd);
    test_assert("A's frequency", hd.codes['A'].occurrences == 6);
    test_assert("B's frequency", hd.codes['B'].occurrences == 1);
    test_assert("C's frequency", hd.codes['C'].occurrences == 6);
    test_assert("D's frequency", hd.codes['D'].occurrences == 2);
    test_assert("E's frequency", hd.codes['E'].occurrences == 5);
    test_assert("F's frequency", hd.codes['F'].occurrences == 0);
    test_assert("Root sum", hd.root->sum == 20);
    test_assert("Root left sum", hd.root->left->sum == 12);
    test_assert("Root right sum", hd.root->right->sum == 8);
    test_assert("Root left left char", hd.root->left->left->codeword->word == 'A');
    test_assert("Root left right char", hd.root->left->right->codeword->word == 'C');
    test_assert("Root right left char", hd.root->right->left->codeword->word == 'E');
    test_assert("Root right right sum", hd.root->right->right->sum == 3);
    test_assert("Root right right left char", hd.root->right->right->right->codeword->word == 'B');
    test_assert("Root right right right char", hd.root->right->right->left->codeword->word == 'D');

    huffman_build_dictionary(&hd);
    test_assert("A's code", hd.codes['A'].bitcode.array[0] == 0b00);
    test_assert("A's length", hd.codes['A'].bitcode.length == 2);
    test_assert("B's code", hd.codes['B'].bitcode.array[0] == 0b111);
    test_assert("B's length", hd.codes['B'].bitcode.length == 3);
    test_assert("C's code", hd.codes['C'].bitcode.array[0] == 0b10);
    test_assert("C's length", hd.codes['C'].bitcode.length == 2);
    test_assert("E's code", hd.codes['E'].bitcode.array[0] == 0b01);
    test_assert("E's length", hd.codes['E'].bitcode.length == 2);
    test_assert("D's code", hd.codes['D'].bitcode.array[0] == 0b011);
    test_assert("D's length", hd.codes['D'].bitcode.length == 3);


    // A    01000001
    // B    01000010
    // C    01000011
    // D    01000100
    // E    01000101
    // Dictionary code: 001A1C01E01D1B
    test_assert("Tree code length", hd.tree_code.length == 49);
    test_assert("Tree code 0", hd.tree_code.array[0] == 0b00001100); //01A
    test_assert("Tree code 1", hd.tree_code.array[1] == 0b00111010); //A1C
    test_assert("Tree code 2", hd.tree_code.array[2] == 0b01100100); //C01E
    test_assert("Tree code 3", hd.tree_code.array[3] == 0b10010001); //E01
    test_assert("Tree code 4", hd.tree_code.array[4] == 0b01000100); //D
    test_assert("Tree code 5", hd.tree_code.array[5] == 0b10000101); //1B
    test_assert("Tree code 6", hd.tree_code.array[6] == 0b0);        //B
    huffman_free_dictionary(&hd);


    size_t output_length;
    huffman_encode(input, input_length, buffer, &output_length);

    test_assert("Encoded 0", buffer[0] == 0b00001100);
    test_assert("Encoded 1", buffer[1] == 0b00111010);
    test_assert("Encoded 2", buffer[2] == 0b01100100);
    test_assert("Encoded 3", buffer[3] == 0b10010001);
    test_assert("Encoded 4", buffer[4] == 0b01000100);
    test_assert("Encoded 5", buffer[5] == 0b10000101);
    test_assert("Encoded 6", buffer[6] == 0b00000000);
    test_assert("Encoded 7", buffer[7] == 0b11100000);
    test_assert("Encoded 8", buffer[8] == 0b10101010);
    test_assert("Encoded 9", buffer[9] == 0b10111010);
    test_assert("Encoded 10", buffer[10] == 0b01010101);
    test_assert("Encoded 11", buffer[11] == 0b0101);
}

void test_huffman_decode(){
    huffman_dictionary hd;
    huffman_init(&hd);

    bitcode tree_code;
    bitcode_init(&tree_code);
    bitcode_store_byte(0b00001100, &tree_code);
    bitcode_store_byte(0b00111010, &tree_code);
    bitcode_store_byte(0b01100100, &tree_code);
    bitcode_store_byte(0b10010001, &tree_code);
    bitcode_store_byte(0b01000100, &tree_code);
    bitcode_store_byte(0b10000101, &tree_code);
    bitcode_store_byte(0b0, &tree_code);
    hd.root = huffman_reconstruct_tree(&tree_code, &hd);

    test_assert("Root left left char", hd.root->left->left->codeword->word == 'A');
    test_assert("Root left right char", hd.root->left->right->codeword->word == 'C');
    test_assert("Root right left char", hd.root->right->left->codeword->word == 'E');
    test_assert("Root right right left char", hd.root->right->right->right->codeword->word == 'B');
    test_assert("Root right right right char", hd.root->right->right->left->codeword->word == 'D');
    
    byte data[] = { 0b00001100,
                    0b00111010,
                    0b01100100,
                    0b10010001,
                    0b01000100,
                    0b10000101,
                    0b00000000,
                    0b11100000,
                    0b10101010,
                    0b10111010,
                    0b01010101,
                    0b0101 };

    size_t output_length;
    char output[20];
    huffman_decode(data, 12, (byte *) output, 20, &output_length);

    test_assert("Output length not correct", output_length == 20);
    test_assert("Output string not correct", strncmp(output, "AAAAAABCCCCCCDDEEEEE", output_length) == 0);

    huffman_free_dictionary(&hd);
    bitcode_free(&tree_code);
}

void test_huffman(){
    test_huffman_encode();
    test_huffman_decode();
}