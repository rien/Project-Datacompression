//
// Created by rien on 10/19/16.
//

#include <malloc.h>
#include <stdlib.h>
#include <assert.h>
#include "priorityqueue.h"

int huffman_comparator(const void* a, const void* b){
    size_t ha = (*(huffman_node**)a)->sum;
    size_t hb = (*(huffman_node**)b)->sum;
    return (ha < hb) ? -1 : (int)(ha - hb);
}

void pq_free(priorityqueue *pq) {
    free(pq->items);
    free(pq);
}

void pq_insert(PQ_TYPE *item, priorityqueue *pq) {
    pq_quick_insert(item, pq);
    pq_sort(pq);
}

void pq_sort(priorityqueue *pq) {
    qsort(pq->items, pq->size, sizeof(PQ_TYPE*), huffman_comparator);
}

void pq_quick_insert(PQ_TYPE *item, priorityqueue *pq) {
    assert(pq->size < PQ_MAX_SIZE);
    pq->items[pq->size++] = item;
}

PQ_TYPE* pq_peek_last(priorityqueue *pq) {
    assert(pq->size != 0);
    return pq->items[pq->size-1];
}

PQ_TYPE *pq_remove_last(priorityqueue *pq) {
    assert(pq->size != 0);
    pq->size--;
    return pq->items[pq->size];
}
