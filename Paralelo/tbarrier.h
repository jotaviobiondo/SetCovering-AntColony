#ifndef TBARRIER_H
#define TBARRIER_H

#include <stdbool.h>
#include <pthread.h>

typedef struct Node node_t;
typedef struct TreeBarrier tree_barrier_t;

struct Node {
    int count;
    node_t *parent;
    volatile bool sense;
    pthread_mutex_t *lock;
    pthread_cond_t *cond;
};

struct TreeBarrier {
    int radix;
    node_t **leaf;
    int leaves;
};

tree_barrier_t* tree_barrier_create(int n);
void tree_barrier_await(tree_barrier_t *b, int id, bool *threadSense);

#endif
