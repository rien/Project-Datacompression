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
    // If a node is a leaf (it has a codeword) it should not be freed,
    // leaves are part of the huffman_dictionary struct.
    if(!root->codeword){
        free(root);
    }
}

void huffman_dictionary_free(huffman_dictionary *dict) {
    huffman_tree_free(dict->root);
    free(dict);
}

huffman_node* huffman_node_parent(huffman_node* left, huffman_node* right){
    huffman_node* parent = malloc(sizeof(huffman_node));
    parent->left = left;
    parent->right = right;
    parent->codeword = NULL;
    parent->sum = left->sum + right->sum;
    return parent;
}

/**
 * Initialize the huffman dictionary by setting
 */
void huffman_init_dictionary(huffman_dictionary* hd) {
    for (size_t i = 0; i < 256; ++i) {
        hd->codes[i].word = UCHAR(i);
        hd->leaves[i].codeword = &hd->codes[i];
    }
}

/**
 * Build a huffman tree for the input.
 */
void huffman_build_tree(uchar *input, size_t length, huffman_dictionary *hd) {
    priorityqueue pq;

    // Count the characters
    for (size_t i = 0; i < length; ++i) {
        hd->codes[input[i]].occurrences++;
    }

    // Insert the leaves in a priorityqueue
    for (size_t j = 0; j < 256; ++j) {
        hd->leaves[j].sum = hd->codes[j].occurrences;
        pq_quick_insert(&hd->leaves[j], &pq);
    }
    pq_sort(&pq);

    // Huffman algorithm: merge the two nodes with the lowest frequency and put the new node back
    // with the sum of the two frequencies. Do this until there is only one node left (the root).
    while(pq.size > 1){
        pq_insert(
                huffman_node_parent(
                        pq_remove_last(&pq),
                        pq_remove_last(&pq)),
                &pq);
    }
    hd->root = pq_remove_last(&pq);
}

void huffman_build_dictionary(huffman_dictionary *hd) {

}
