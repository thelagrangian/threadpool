#include <error/error_macro.h>
#include <constants.h>
#include <threadpool.h>


void *thread_start(void* start_arg){

    int s;

    start_arg_t* arg = (start_arg_t*)start_arg;

    pthread_mutex_t *mutex = arg->mutex;
    pthread_cond_t *cond = arg->cond;

    int *to_join = arg->to_join;
    circularqueue_t *task_queue = arg->task_queue;
    circularqueue_t *argt_queue = arg->argt_queue;
    callback_t callback = NULL;
    void *callback_arg = NULL;

    struct timespec to;

    while(!*arg->to_join){

        s = pthread_mutex_lock(mutex);
        POSIX_CHECK(s);

        while(circularqueue_empty(task_queue)) {

            clock_gettime(CLOCK_MONOTONIC, &to);
            to.tv_sec += cond_wait_timeout_sec;

            s = pthread_cond_timedwait(cond, mutex, &to);
            if (s != ETIMEDOUT) {
                POSIX_CHECK(s);
            }
            if(*arg->to_join) {
                break;
            }
        }

        if(!circularqueue_empty(task_queue)) {
            callback = circularqueue_front(task_queue);
            callback_arg = circularqueue_front(argt_queue);
            circularqueue_pop_front(task_queue);
            circularqueue_pop_front(argt_queue);
        }

        s = pthread_mutex_unlock(mutex);
        POSIX_CHECK(s);

        if(callback != NULL) {
            callback(callback_arg);
            callback = NULL;
            callback_arg = NULL;
        }

    }

    return start_arg;
}

int threadpool_init(threadpool_t *threadpool, int num_threads, int num_tasks,
                    const pthread_attr_t *attr,
                    const pthread_mutexattr_t *mutexattr) {

    int s;

    threadpool->to_join = 0;

    threadpool->num_threads = num_threads;
    threadpool->num_tasks = num_tasks;

    circularqueue_init(&threadpool->task_queue, num_tasks);
    circularqueue_init(&threadpool->argt_queue, num_tasks);

    threadpool->threads = (pthread_t*) malloc(num_threads * sizeof(pthread_t));
    threadpool->attr = attr;
    threadpool->mutexattr = mutexattr;

    s = pthread_mutex_init(&threadpool->mutex, threadpool->mutexattr);
    POSIX_CHECK(s);

    s = pthread_condattr_init(&threadpool->condattr);
    POSIX_CHECK(s);
    s = pthread_condattr_setclock(&threadpool->condattr, CLOCK_MONOTONIC);
    POSIX_CHECK(s);
    s = pthread_cond_init(&threadpool->cond, &threadpool->condattr);
    POSIX_CHECK(s);

    return 0;

}

int threadpool_init_default(threadpool_t *threadpool, int num_threads, int num_tasks) {

    return threadpool_init(threadpool, num_threads, num_tasks, NULL, NULL);
}

int threadpool_create(threadpool_t *threadpool) {
    int num_threads = threadpool->num_threads;

    threadpool->start_arg.to_join = &threadpool->to_join;
    threadpool->start_arg.mutex = &threadpool->mutex;
    threadpool->start_arg.cond = &threadpool->cond;
    threadpool->start_arg.task_queue = &threadpool->task_queue;
    threadpool->start_arg.argt_queue = &threadpool->argt_queue;

    int s, i;
    for(i = 0; i < num_threads; ++i) {
        s = pthread_create(threadpool->threads + i, threadpool->attr, thread_start, &threadpool->start_arg);
        POSIX_CHECK(s);
    }

    return 0;
}

int threadpool_submit(threadpool_t *threadpool, callback_t cb, void *cb_arg) {

    pthread_mutex_t *mutex = &threadpool->mutex;
    pthread_cond_t *cond = &threadpool->cond;

    int ret, s;
    s = pthread_mutex_lock(mutex);
    POSIX_CHECK(s);

    if(circularqueue_full(&threadpool->task_queue)) {
        ret = 1;
    } else {
        circularqueue_push_back(&threadpool->task_queue, cb);
        circularqueue_push_back(&threadpool->argt_queue, cb_arg);
        ret = 0;
        s = pthread_cond_broadcast(cond);
        POSIX_CHECK(s);
    }

    s = pthread_mutex_unlock(mutex);
    POSIX_CHECK(s);

    return ret;
}

int threadpool_join(threadpool_t *threadpool) {

    threadpool->to_join = 1;

    int num_threads = threadpool->num_threads;
    int s, i;

    for(i = 0; i < num_threads; ++i) {
        s = pthread_join(threadpool->threads[i], NULL);
        POSIX_CHECK(s);
    }

    return 0;
}

int threadpool_destroy(threadpool_t *threadpool) {

    int ret = 0, s;

    free(threadpool->threads);
    circularqueue_destroy(&threadpool->task_queue);
    circularqueue_destroy(&threadpool->argt_queue);

    s = pthread_mutex_destroy(&threadpool->mutex);
    POSIX_CHECK(s);
    s = pthread_cond_destroy(&threadpool->cond);
    POSIX_CHECK(s);
    s = pthread_condattr_destroy(&threadpool->condattr);
    POSIX_CHECK(s);

    return ret;
}
