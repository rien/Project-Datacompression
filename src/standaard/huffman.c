//
// Created by rien on 10/20/16.
//

#include "huffman.h"

void huffman_tree_free(huffman_node *root) {
    if(root->left){
        huffman_tree_free(root->left);
    }
    if(root->right){
        huffman_tree_free(root->right);
    }
    if(root->codeword){
        free(root->codeword);
    }
    free(root);
}

void huffman_dictionary_free(huffman_dictionary *dict) {
    huffman_tree_free(dict->root);
    free(dict);
}
