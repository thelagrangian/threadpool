#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include <time.h>
#include <unistd.h>

typedef void* (*callback_t)(void*);

struct start_arg_t {
    int *to_join;
    int num_tasks;

    pthread_mutex_t *mutex;
    pthread_cond_t *cond;

    callback_t *task_func_queue;
    void **task_arg_queue;
    int *beginidx;
    int *endidx;
};

typedef struct start_arg_t start_arg_t;

struct threadpool_t {
    int to_join;
    int num_threads;
    int num_tasks;

    callback_t *task_func_queue;
    void **task_arg_queue;
    int beginidx;
    int endidx;

    pthread_t *threads;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    pthread_condattr_t condattr;

    start_arg_t start_arg;
};

typedef struct threadpool_t threadpool_t;

void * thread_start(void* start_arg){

    start_arg_t* arg = (start_arg_t*)start_arg;

    pthread_mutex_t *mutex = arg->mutex;
    pthread_cond_t *cond = arg->cond;

    int *to_join = arg->to_join;
    int *beginidx = arg->beginidx;
    int *endidx = arg->endidx;
    callback_t callback = NULL;
    void **callback_arg = NULL;
    int num_tasks = arg->num_tasks;

    struct timespec to;

    while(!*arg->to_join){

        printf("outer\n");

        clock_gettime(CLOCK_MONOTONIC, &to);
        to.tv_sec += 1;

        pthread_mutex_lock(mutex);
        // while(!*arg->to_join && *beginidx == *endidx) {
        //     printf("before\n");
        //     pthread_cond_timedwait(cond, mutex, &to);
	//     printf("after\n");
        // }
	//
	while(*beginidx == *endidx) {
            pthread_cond_timedwait(cond, mutex, &to);
            if(*arg->to_join) {
                printf("to_join\n");
                break;
            }
        }

        if(*beginidx != *endidx){
            callback = arg->task_func_queue[*beginidx];
            callback_arg = arg->task_arg_queue[*beginidx];
            *beginidx = (*beginidx + 1) % (num_tasks + 1);
        }

        pthread_mutex_unlock(mutex);

        if(callback != NULL) {
            callback(callback_arg);
            callback = NULL;
            callback_arg = NULL;
        }

    }

    return NULL;
}

int threadpool_init(threadpool_t *threadpool, int num_threads, int num_tasks){

    threadpool->to_join = 0;

    threadpool->num_threads = num_threads;
    threadpool->num_tasks = num_tasks;

    threadpool->beginidx = 0;
    threadpool->endidx = 0;

    threadpool->task_func_queue = (callback_t*) malloc(num_tasks * sizeof(callback_t));
    threadpool->task_arg_queue = (void**) malloc(num_tasks * sizeof(void*));

    threadpool->threads = (pthread_t*) malloc(num_threads * sizeof(pthread_t));

    pthread_mutex_init(&threadpool->mutex, NULL);
    pthread_condattr_init(&threadpool->condattr);
    pthread_condattr_setclock(&threadpool->condattr, CLOCK_MONOTONIC);
    pthread_cond_init(&threadpool->cond, &threadpool->condattr);

    return 0;
}

int threadpool_create(threadpool_t *threadpool) {
    int num_threads = threadpool->num_threads;

    threadpool->start_arg.to_join = &threadpool->to_join;
    threadpool->start_arg.num_tasks = threadpool->num_tasks;
    threadpool->start_arg.mutex = &threadpool->mutex;
    threadpool->start_arg.cond = &threadpool->cond;
    threadpool->start_arg.task_func_queue = threadpool->task_func_queue;
    threadpool->start_arg.task_arg_queue = threadpool->task_arg_queue;
    threadpool->start_arg.beginidx = &threadpool->beginidx;
    threadpool->start_arg.endidx = &threadpool->endidx;

    int ret = 0;
    int i;
    for(i = 0; i < num_threads; ++i) {
        ret = ret || pthread_create(threadpool->threads + i, NULL, thread_start, &threadpool->start_arg);
    }

    // usleep(5000000);

    return ret;
}

int threadpool_submit(threadpool_t *threadpool, callback_t cb, void *cb_arg) {

    pthread_mutex_t *mutex = &threadpool->mutex;
    pthread_cond_t *cond = &threadpool->cond;
    int *beginidx = &threadpool->beginidx;
    int *endidx = &threadpool->endidx;
    int num_tasks = threadpool->num_tasks;

    int ret;
    pthread_mutex_lock(mutex);

    if((*endidx + 1) % (num_tasks + 1) == *beginidx) {
        ret = -1;
    } else {
        threadpool->task_func_queue[*endidx] = cb;
        threadpool->task_arg_queue[*endidx] = cb_arg;
        *endidx = (*endidx + 1) % (num_tasks + 1);
        ret = 0;
        pthread_cond_broadcast(cond);
    }

    pthread_mutex_unlock(mutex);

    return ret;
}

int threadpool_join(threadpool_t *threadpool) {

    threadpool->to_join = 1;

    int num_threads = threadpool->num_threads;
    int i;

    int ret = 0;
    for(i = 0; i < num_threads; ++i) {
        ret = ret || pthread_join(threadpool->threads[i], NULL);
    }

    return ret;
}

int threadpool_destroy(threadpool_t *threadpool) {

    free(threadpool->threads);
    free(threadpool->task_func_queue);
    free(threadpool->task_arg_queue);

    pthread_mutex_destroy(&threadpool->mutex);
    pthread_cond_destroy(&threadpool->cond);
    pthread_condattr_destroy(&threadpool->condattr);

    return 0;
}

#endif // THREADPOOL_H
