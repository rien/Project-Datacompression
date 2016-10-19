//
// Created by rien on 10/19/16.
//

#include <malloc.h>
#include "priorityqueue.h"

priorityqueue *pq_allocate(size_t max_size, int (*comparator)(const void *, const void *)) {
    priorityqueue* pq = malloc(sizeof(priorityqueue));
    pq->comparator = comparator;
    pq->size = 0;
    pq->max_size = max_size;
    pq->items = malloc(sizeof(void*)*max_size);
    return pq;
}

void pq_free(priorityqueue *pq) {
    free(pq->items);
    free(pq);
}

void pq_insert(void *item, priorityqueue *pq) {

}
