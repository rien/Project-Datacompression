//
// Created by rien on 10/19/16.
//

#ifndef DA3_PROJECT_PRIORITYQUEUE_H
#define DA3_PROJECT_PRIORITYQUEUE_H

#include <stddef.h>

typedef struct {
    void* items;
    size_t size;
    size_t max_size;
    int (*comparator)(const void*,const void*);
} priorityqueue;

priorityqueue* pq_allocate(size_t max_size, int (*comparator)(const void*,const void*));

void pq_insert(void* item, priorityqueue* pq);

void pq_insert_witouth_sorting(void* item, priorityqueue* pq);

void pq_fix_sorting(priorityqueue* pq);

void* pq_peek(priorityqueue* pq);

void* pq_remove(priorityqueue* pq);

void pq_free(priorityqueue* pq);

#endif //DA3_PROJECT_PRIORITYQUEUE_H
