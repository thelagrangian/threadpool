#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <threadpool.h>

/*
typedef void* (*start_func_t)(void*);

struct start_arg_t{
    int payload;
};

typedef struct start_arg_t start_arg_t;

void* startfunc(void* start_arg){
    start_arg_t* arg = (start_arg_t*)start_arg;
    printf("%d hello\n", (int)arg->payload);
    return 0;
}
*/

int main(){
    /*
    int queuesize = 5;
    start_func_t *funcp;
    start_arg_t **argp;
    funcp = (start_func_t*)malloc(queuesize*sizeof(start_func_t));
    argp = (start_arg_t**)malloc(queuesize*sizeof(start_arg_t*));
    start_arg_t arg0;
    arg0.payload = 5;
    funcp[0] = startfunc;
    argp[0] = &arg0;
    funcp[0]((void*)(argp[0]));
    free(funcp);
    free(argp);
    printf("%lu\n", sizeof(void));
    */

    threadpool_t threadpool;
    threadpool_init(&threadpool, 5, 4);
    threadpool_create(&threadpool);

    usleep(2000000);

    threadpool_join(&threadpool);
    threadpool_destroy(&threadpool);
    return 0;
}
