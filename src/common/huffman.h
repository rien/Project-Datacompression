/*
 * Author:
 * Rien Maertens
 * Rien.Maertens@UGent.be
 * 3de Bachelor Informatica
 * UGent
 *
 */

#ifndef DA3_PROJECT_HUFFMAN_H
#define DA3_PROJECT_HUFFMAN_H


#include <stdlib.h>
#include <stdbool.h>
#include "bitcode.h"
#include "common.h"

typedef struct {
    // The word it represents
    byte word;

    // The encoded bitcode
    bitcode bitcode;

    // The amount of occurrences in the text
    size_t occurrences;
} huffman_codeword;

typedef struct huffman_node {
    struct huffman_node* left;
    struct huffman_node* right;
    size_t sum;

    // If this node is a leaf, there should be a codeword present
    // otherwise, codeword should be NULL
    huffman_codeword* codeword;
} huffman_node;

typedef struct {
    huffman_codeword codes[256];
    huffman_node leaves[256];
    huffman_node* root;
    bitcode tree_code;
} huffman_dictionary;

void huffman_init(huffman_dictionary *hd);

void huffman_free_dictionary(huffman_dictionary* hd);

void huffman_build_dictionary(huffman_dictionary *hd);

void huffman_build_tree(byte *input, size_t length, huffman_dictionary *hd);

huffman_node* huffman_reconstruct_tree(bitcode* input_bc, huffman_dictionary* hd);

void huffman_encode(byte input[], size_t length, byte *output, size_t *output_length);

void huffman_decode(byte input[], size_t input_length, byte output[], size_t a_encoded, size_t *output_length);


#endif //DA3_PROJECT_HUFFMAN_H