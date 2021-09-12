#include <stdio.h>
#include <circularqueue/circularqueue.h>

int circularqueue_init(circularqueue_t *cqueue, int qcapacity) {

    cqueue->qcapacity = qcapacity;
    cqueue->qsize = 0;
    cqueue->beginidx = 0;
    cqueue->endidx = 0;

    cqueue->queue = malloc(sizeof(void *) * (qcapacity + 1));

    return cqueue->queue != NULL;
}

int circularqueue_full(circularqueue_t *cqueue) {
    return cqueue->qsize == cqueue->qcapacity;
}

int circularqueue_empty(circularqueue_t *cqueue) {
    return cqueue->beginidx == cqueue->endidx;
}

int circularqueue_push_back(circularqueue_t *cqueue, void *item) {
    if(circularqueue_full(cqueue)) {
        return 0;
    } else {
        cqueue->queue[cqueue->endidx] = item;
        cqueue->endidx = (cqueue->endidx + 1) % (cqueue->qcapacity + 1);
        cqueue->qsize++;
        return 1;
    }
}

void *circularqueue_front(circularqueue_t *cqueue) {
    if(circularqueue_empty(cqueue)) {
        return NULL;
    } else {
        return cqueue->queue[cqueue->beginidx];
    }
}

int circularqueue_pop_front(circularqueue_t *cqueue) {
    if(circularqueue_empty(cqueue)) {
        return 0;
    } else {
        cqueue->beginidx = (cqueue->beginidx + 1) % (cqueue->qcapacity + 1);
        cqueue->qsize--;
        return 1;
    }
}

int circularqueue_destroy(circularqueue_t *cqueue) {
    free(cqueue->queue);
}
