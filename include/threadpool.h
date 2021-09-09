#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

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

void *thread_start(void* start_arg);

int threadpool_init(threadpool_t *threadpool, int num_threads, int num_tasks);

int threadpool_create(threadpool_t *threadpool);

int threadpool_submit(threadpool_t *threadpool, callback_t cb, void *cb_arg);

int threadpool_join(threadpool_t *threadpool);

int threadpool_destroy(threadpool_t *threadpool);

#endif // THREADPOOL_H
