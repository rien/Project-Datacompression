//
// Created by rien on 10/21/16.
//

#include "test_priorityqueue.h"
#include "../../src/standaard/huffman.h"
#include "../../src/standaard/priorityqueue.h"
#include "../test_common/testmacro.h"

void test_bulk_insert(){
    huffman_node hn[256];
    priorityqueue pq = PQ_INIT;
    for (size_t i = 0; i < 256; ++i) {
        hn[i].sum = 255-i;
        pq_quick_insert(&hn[i],&pq);
    }
    test_assert("PQ should not be sorted", pq_peek_last(&pq)->sum == 0);
    pq_sort(&pq);
    for (size_t i = 0; i < 256; ++i) {
        test_assert("PQ remove", pq_remove_last(&pq)->sum == (255-i));
    }
}

void test_simple_insert(){
    huffman_node hn[4];
    priorityqueue pq = PQ_INIT;
    for (size_t i = 0; i < 4; ++i) {
        hn[i].sum = i;
    }
    test_assert("PQ should be empty", pq.size == 0);
    pq_insert(&hn[2], &pq);
    test_assert("PQ should contain 2", pq_peek_last(&pq)->sum  == 2);
    pq_insert(&hn[1], &pq);
    test_assert("PQ size should be 2", pq.size  == 2);
    test_assert("PQ last item should be 2", pq_peek_last(&pq)->sum == 2);
    pq_insert(&hn[3], &pq);
    pq_insert(&hn[0], &pq);
    test_assert("PQ last item should be 3", pq_peek_last(&pq)->sum == 3);
    test_assert("PQ size should be 4", pq.size  == 4);
    for (size_t j = 0; j < 4 ; ++j) {
        test_assert("Should remove last item", pq_remove_last(&pq)->sum == (3-j));
    }


}


void test_priorityqueue() {
    test_simple_insert();
    test_bulk_insert();
}