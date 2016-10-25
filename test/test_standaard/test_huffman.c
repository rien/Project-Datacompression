//
// Created by rien on 10/23/16.
//

#include "test_huffman.h"
#include "../../src/standaard/huffman.h"
#include "../../src/common/bitcode.h"

void test_huffman(){
    huffman_dictionary hd;
    huffman_init_dictionary(&hd);

    uchar* input = UCHAR_PTR("AAAAAABCCCCCCDDEEEEE");
    huffman_build_tree(input, strlen((char*)input),&hd);
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
     */
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
    test_assert("A's code", hd.codes['A'].code[0] == 0b00);
    test_assert("A's length", hd.codes['A'].length == 2);
    test_assert("B's code", hd.codes['B'].code[0] == 0b111);
    test_assert("B's length", hd.codes['B'].length == 3);
    test_assert("C's code", hd.codes['C'].code[0] == 0b10);
    test_assert("C's length", hd.codes['C'].length == 2);
    test_assert("E's code", hd.codes['E'].code[0] == 0b01);
    test_assert("E's length", hd.codes['E'].length == 2);
    test_assert("D's code", hd.codes['D'].code[0] == 0b011);
    test_assert("D's length", hd.codes['D'].length == 3);


    // A    01000001
    // B    01000010
    // C    01000011
    // D    01000100
    // E    01000101
    // Dictionary code: 001A1C01E01D1B
    test_assert("Tree code length", hd.tree_code.next_bit == 49);
    test_assert("Tree code 0", hd.tree_code.array[0] == 0b00001100); // 01A
    test_assert("Tree code 1", hd.tree_code.array[1] == 0b00111010); //A1C
    test_assert("Tree code 2", hd.tree_code.array[2] == 0b01100100); //C01E
    test_assert("Tree code 3", hd.tree_code.array[3] == 0b10010001); //E01
    test_assert("Tree code 4", hd.tree_code.array[4] == 0b01000100); //D
    test_assert("Tree code 5", hd.tree_code.array[5] == 0b10000101); //1B
    test_assert("Tree code 6", hd.tree_code.array[6] == 0b0);        //B

    huffman_dictionary_free(&hd);
}