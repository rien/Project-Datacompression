//
// Created by rien on 10/20/16.
//

#ifndef DA3_PROJECT_HUFFMAN_H
#define DA3_PROJECT_HUFFMAN_H


#include <stdlib.h>
#include <stdbool.h>
#include "../common/uchar.h"
#include "../common/bitcode.h"

typedef struct {
    uchar word;
    bitcode bitcode;
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

void huffman_init_dictionary(huffman_dictionary* hd);

void huffman_build_tree(uchar input[], size_t length, huffman_dictionary* hd);

void huffman_build_dictionary(huffman_dictionary* hd);

void huffman_dictionary_free(huffman_dictionary* hd);

uchar* huffman_encode(uchar input[], size_t length, size_t* output_length);

uchar* huffman_decode(uchar input[], size_t length, size_t* output_length);


#endif //DA3_PROJECT_HUFFMAN_H