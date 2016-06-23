#include "tbarrier.h"
#include <stdlib.h>
#include <math.h>

/* NODE */

node_t* createNode(tree_barrier_t *b){
    node_t *node = (node_t*)malloc(sizeof(node_t));
    node->sense = false;
    node->parent = NULL;
    node->count = b->radix;
    node->lock = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(node->lock, NULL);
    node->cond = (pthread_cond_t*) malloc(sizeof(pthread_cond_t));
    pthread_cond_init(node->cond, NULL);
    return node;
}

node_t* createNodeParent(tree_barrier_t *b, node_t* myParent){
    node_t* node = createNode(b);
    node->parent = myParent; 
    return node;
}

void await(tree_barrier_t *b, node_t *node, bool *threadSense){
    bool mySense = *threadSense;
    int position;
    pthread_mutex_lock(node->lock);
    position = node->count--;
    pthread_mutex_unlock(node->lock);
    if (position == 1){
        if (node->parent != NULL){
            await(b, node->parent, threadSense);
        }
        pthread_mutex_lock(node->lock);
        node->sense = mySense;
        node->count = b->radix;
        pthread_mutex_unlock(node->lock);
        pthread_cond_broadcast(node->cond);
    } else {
        pthread_mutex_lock(node->lock);
        if (node->sense != mySense) {
            pthread_cond_wait(node->cond, node->lock);
        }
        pthread_mutex_unlock(node->lock);
    }
    *threadSense = !mySense;
}

/* TREE BARRIER */

//R^x = N --> achar valor de R inteiro para o menor x
int calcRadix(int N){
    double r;
    int i;
    for (i = 2; i <= N; ++i){
        r = log(N)/log(i);
        if (ceil(r) == r){
            return i;
        }
    }
    return 1;
}

void build(tree_barrier_t *b, node_t *parent, int depth){
    if (depth <= 0){
        b->leaf[b->leaves++] = parent;
    } else {
        int i;
        for (i = 0; i < b->radix; ++i){
            node_t *child = createNodeParent(b, parent);
            build(b, child, depth - 1);
        }
    }
}

tree_barrier_t* tree_barrier_create(int n){
    tree_barrier_t* b = (tree_barrier_t*)malloc(sizeof(tree_barrier_t));

    b->radix = calcRadix(n);
    b->leaves = 0;
    b->leaf = (node_t**)calloc(n / b->radix, sizeof(node_t*));
    int depth = 0;

    while (n > 1){
        depth++;
        n = n / b->radix;
    }

    node_t *root = createNode(b);
    build(b, root, depth - 1);
    return b;
}

void tree_barrier_await(tree_barrier_t *b, int id, bool *threadSense){
    int me = id;
    node_t *myLeaf = b->leaf[me / b->radix];
    await(b, myLeaf, threadSense);
}
