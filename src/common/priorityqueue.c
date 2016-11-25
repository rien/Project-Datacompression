//
// Created by rien on 10/19/16.
//

#include <malloc.h>
#include <stdlib.h>
#include <assert.h>
#include "priorityqueue.h"

/**
 *  Compare two huffman nodes so this function can be used
 *  to sort in descending mode.
 */
int huffman_comparator(const void* a, const void* b){
    size_t ha = (*(huffman_node**)a)->sum;
    size_t hb = (*(huffman_node**)b)->sum;
    return (hb < ha) ? -1 : (int)(hb - ha);
}

/**
 * Insert an item in the priorityqueue and keep it is a sorted order.
 */
void pq_insert(PQ_TYPE *item, priorityqueue *pq) {
    pq_quick_insert(item, pq);
    pq_sort(pq);
}

/**
 * Sort the pq, because the size is never > 256, the execution time is constant.
 */
void pq_sort(priorityqueue *pq) {
    qsort(pq->items, pq->size, sizeof(PQ_TYPE*), huffman_comparator);
}

/**
 * Insert an item without sorting. Use this when inserting more than one item.
 * After inserting the desired items, pq_sort() should be used.
 */
void pq_quick_insert(PQ_TYPE *item, priorityqueue *pq) {
    assert(pq->size < PQ_MAX_SIZE);
    pq->items[pq->size++] = item;
}

/**
 * Return the item with the smallest key in the queue.
 */
PQ_TYPE* pq_peek_last(priorityqueue *pq) {
    assert(pq->size != 0);
    return pq->items[pq->size-1];
}

/**
 * Return the item with the smallest key, and delete it from the queue.
 */
PQ_TYPE *pq_remove_last(priorityqueue *pq) {
    assert(pq->size != 0);
    pq->size--;
    return pq->items[pq->size];
}
