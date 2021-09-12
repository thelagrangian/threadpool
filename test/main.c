#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <threadpool.h>

void *sample(void* arg) {
    int *ptr = (int *)arg;
    printf("hello world %d\n", *ptr);
    return NULL;
}

int main(){

    int num = 1024;

    threadpool_t threadpool;
    threadpool_init_default(&threadpool, 5, 4);
    threadpool_create(&threadpool);
    threadpool_submit(&threadpool, sample, (void *)&num);

    usleep(10000000);

    threadpool_join(&threadpool);
    threadpool_destroy(&threadpool);
    return 0;
}
