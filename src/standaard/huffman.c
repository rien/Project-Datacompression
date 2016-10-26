//
// Created by rien on 10/20/16.
//

#include <assert.h>
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
    if(root->codeword){
        bitcode_free(&root->codeword->bitcode);
    } else {
        free(root);
    }
}

void huffman_free_dictionary(huffman_dictionary *dict) {
    huffman_tree_free(dict->root);
    bitcode_free(&dict->tree_code);
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
 * Build a huffman tree for the input.
 */
void huffman_build_tree(uchar *input, size_t length, huffman_dictionary *hd) {
    priorityqueue pq = PQ_INIT;

    // Count the characters
    for (size_t i = 0; i < length; ++i) {
        hd->codes[input[i]].occurrences++;
    }

    // Insert the leaves in a priorityqueue
    for (size_t j = 0; j < 256; ++j) {
        hd->leaves[j].sum = hd->codes[j].occurrences;
        if(hd->leaves[j].sum > 0){
            pq_quick_insert(&hd->leaves[j], &pq);
        }
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


/**
 * Traverse a huffman tree and set the bitcode of each codeword in the tree.
 */
void traverse_tree(huffman_node* root, bitcode* word_code, bitcode* tree_code, bool is_right){
    bitcode_store_bit(is_right, word_code);
    if(root->codeword){
        // we reached a leaf, write the code
        bitcode_copy(word_code, &root->codeword->bitcode);

        // write to the tree code: 1 + the character
        bitcode_store_bit(true, tree_code);
        bitcode_store_byte(root->codeword->word, tree_code);
    } else {
        // we do not have a leaf, write 0 to the tree code
        bitcode_store_bit(false, tree_code);

        // go deeper in the tree
        traverse_tree(root->left, word_code, tree_code, false);
        traverse_tree(root->right, word_code, tree_code, true);
    }
    bitcode_clear_one(word_code);
}

/**
 * Set the bitcodes of the codewords in the huffman-tree while also constructing
 * the code to store the tree itself.
 *
 * The tree itself is stored by traversing the tree in pre-order:
 *   - if the current node is a leaf, output 1, followed by the character it represents
 *   - if the current node is not a leaf, output 0 and visit the children
 */
void huffman_build_dictionary(huffman_dictionary *hd) {
    bitcode bc;
    bitcode_init(&bc);
    assert(hd->root);
    if(hd->root->codeword ){
        // There is only 1 codeword
        bitcode_store_bit(true, &hd->tree_code);
        bitcode_store_byte(hd->root->codeword->word, &hd->tree_code);

        //code is just 0
        bitcode_init(&hd->root->codeword->bitcode);
        bitcode_store_bit(false, &hd->root->codeword->bitcode);
    } else {
        bitcode_store_bit(false, &hd->tree_code);
        traverse_tree(hd->root->left, &bc, &hd->tree_code, false);
        traverse_tree(hd->root->right, &bc, &hd->tree_code, true);
    }
    bitcode_free(&bc);
}

/**
 * Initialize the huffman dictionary by setting.
 */
void huffman_init_dictionary(uchar *input, size_t length, huffman_dictionary *hd) {
    for (size_t i = 0; i < 256; ++i) {
        huffman_codeword* hdc = &hd->codes[i];
        hdc->word = UCHAR(i);
        hdc->occurrences = 0;

        huffman_node* hdl = &hd->leaves[i];
        hdl->codeword = hdc;
        hdl->left = NULL;
        hdl->right = NULL;
    }
    hd->root = NULL;
    bitcode_init(&hd->tree_code);
    huffman_build_tree(input, length, hd);
    huffman_build_dictionary(hd);
}

uchar *huffman_encode(uchar *input, size_t length, size_t *output_length) {
    huffman_dictionary hd;
    huffman_init_dictionary(input, length, &hd);

    unsigned short int tree_code_length = (unsigned short int) hd.tree_code.length;

    bitcode output;
    bitcode_init(&output);

    // lower bits
    bitcode_store_byte((uchar) tree_code_length, &output);

    // higher bits
    bitcode_store_byte((uchar) tree_code_length << 8, &output);

    // tree code itself
    bitcode_append(&hd.tree_code, &output);

    for (size_t i = 0; i < length; ++i) {
        bitcode_append(&hd.codes[input[i]].bitcode, &output);
    }
    *output_length = ((output.length-1)/8)+1;
    size_t malloc_size = sizeof(uchar)*(*output_length);
    uchar* output_str = calloc(*output_length, sizeof(uchar));
    bitcode_write_all(output_str, NULL, &output);

    huffman_free_dictionary(&hd);
    bitcode_free(&output);
    return output_str;
}
