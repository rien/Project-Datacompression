//
// Created by rien on 10/20/16.
//

#include "huffman.h"
#include "priorityqueue.h"

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

huffman_dictionary *huffman_build_dictionary(uchar *input, size_t length) {
    huffman_dictionary* hd = huffman_init_dictionary();
    priorityqueue pq;
    for (size_t i = 0; i < length; ++i) {
        hd->codes[input[i]].occurrences++;
    }
    huffman_node* hn = calloc(sizeof(huffman_node), 256);
    for (size_t j = 0; j < 256; ++j) {
        hn[j].codeword = &hd->codes[j];
        hn[j].sum = hd->codes[j].occurrences;
        pq_quick_insert(hn,&pq);
    }
    pq_sort(&pq);
}

huffman_dictionary *huffman_init_dictionary() {
    huffman_dictionary* hd = calloc(sizeof(huffman_dictionary), 1);
    for (size_t i = 0; i < 256; ++i) {
        hd->codes[i].word = UCHAR(i);
    }
    return hd;
}
