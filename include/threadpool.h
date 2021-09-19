#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>

#include <circularqueue/circularqueue.h>

typedef void* (*callback_t)(void*);

struct start_arg_t {

    int *to_join;

    pthread_mutex_t *mutex;
    pthread_cond_t *cond;

    circularqueue_t *task_queue;
    circularqueue_t *argt_queue;
};

typedef struct start_arg_t start_arg_t;

struct threadpool_t {

    int to_join;
    int num_threads;
    int num_tasks;
    unsigned int main_thread_cpu;
    unsigned int main_thread_node;

    circularqueue_t task_queue;
    circularqueue_t argt_queue;

    pthread_t *threads;
    const pthread_attr_t *attr;
    const pthread_mutexattr_t *mutexattr;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    pthread_condattr_t condattr;

    start_arg_t start_arg;
};

typedef struct threadpool_t threadpool_t;

void *thread_start(void* start_arg);

int threadpool_init(threadpool_t *threadpool, int num_threads, int num_tasks,
                    const pthread_attr_t *attr,
                    const pthread_mutexattr_t *mutexattr);

int threadpool_init_default(threadpool_t *threadpool, int num_threads, int num_tasks);

int threadpool_create(threadpool_t *threadpool);

int threadpool_submit(threadpool_t *threadpool, callback_t cb, void *cb_arg);

int threadpool_join(threadpool_t *threadpool);

int threadpool_destroy(threadpool_t *threadpool);

#endif // THREADPOOL_H
