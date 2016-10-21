//
// Created by rien on 10/20/16.
//

#ifndef DA3_PROJECT_HUFFMAN_H
#define DA3_PROJECT_HUFFMAN_H


#include <stdlib.h>
#include <stdbool.h>
#include "../common/uchar.h"

typedef struct {
    uchar word;
    uchar code[32]; // A huffman code is maximum 255 characters long
    size_t length;
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
    size_t size;
    huffman_node* root;
} huffman_dictionary;

huffman_dictionary* huffman_init_dictionary();

huffman_dictionary* huffman_build_dictionary(uchar input[], size_t length);

void huffman_dictionary_free(huffman_dictionary* dict);

void huffman_tree_free(huffman_node* root);

uchar* huffman_encode(uchar input[], size_t length, size_t* output_length, huffman_node* tree);

uchar* huffman_decode(uchar input[], size_t length, size_t* output_length, huffman_node* tree);


#endif //DA3_PROJECT_HUFFMAN_H