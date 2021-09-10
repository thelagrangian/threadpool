#ifndef CIRCULARQUEUE_H
#define CIRCULARQUEUE_H


#include <stdlib.h>

// Here is a better implementation, https://aticleworld.com/implement-vector-in-c/
struct circularqueue_t {

    void **queue;
    int qsize;
    int qcapacity;
    int beginidx;
    int endidx;

};

typedef struct circularqueue_t circularqueue_t;

int circularqueue_init(circularqueue_t *cqueue, int qcapacity);

int circularqueue_full(circularqueue_t *cqueue);

int circularqueue_empty(circularqueue_t *cqueue);

int circularqueue_push_back(circularqueue_t *cqueue, void *item);

void *circularqueue_front(circularqueue_t *cqueue);

int circularqueue_pop_front(circularqueue_t *cqueue);

int circularqueue_destroy(circularqueue_t *cqueue);

#endif // CIRCULARQUEUE_H
