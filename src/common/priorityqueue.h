/*
 * Author:
 * Rien Maertens
 * Rien.Maertens@UGent.be
 * 3de Bachelor Informatica
 * UGent
 *
 */

#ifndef DA3_PROJECT_PRIORITYQUEUE_H
#define DA3_PROJECT_PRIORITYQUEUE_H

#include <stddef.h>
#include "huffman.h"

#define PQ_TYPE huffman_node
#define PQ_MAX_SIZE 256
#define PQ_INIT {.size = 0}

typedef struct {
    PQ_TYPE* items[PQ_MAX_SIZE];
    size_t size;
} priorityqueue;

void pq_insert(PQ_TYPE* item, priorityqueue* pq);

void pq_quick_insert(PQ_TYPE* item, priorityqueue* pq);

void pq_sort(priorityqueue* pq);

PQ_TYPE* pq_peek_last(priorityqueue* pq);

PQ_TYPE* pq_remove_last(priorityqueue* pq);

#endif //DA3_PROJECT_PRIORITYQUEUE_H
