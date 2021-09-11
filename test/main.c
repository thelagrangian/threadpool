#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <threadpool.h>

void* sample(void* arg){
    printf("hello world\n");
    return NULL;
}

int main(){

    threadpool_t threadpool;
    threadpool_init(&threadpool, 5, 4);
    threadpool_create(&threadpool);
    threadpool_submit(&threadpool, sample, NULL);

    usleep(2000000);

    threadpool_join(&threadpool);
    threadpool_destroy(&threadpool);
    return 0;
}
