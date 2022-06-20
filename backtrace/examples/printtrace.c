#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

extern void moniter_signal();

void *thr_fn(void *arg)
{
    char *p = (char *)malloc(1000);
    free(p);
    free(p);
}

int main()
{
    moniter_signal();
    pthread_t tid;
    pthread_create(&tid, NULL, thr_fn, NULL);

    pthread_join(tid,NULL);
    return 0;
}