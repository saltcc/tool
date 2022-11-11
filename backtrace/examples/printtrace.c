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
    char *env_path = (char *)getenv("HOME");
    printf("\nstack trace print in [%s/log/]\n\n", env_path);

    moniter_signal();
    pthread_t tid;
    pthread_create(&tid, NULL, thr_fn, NULL);

    pthread_join(tid,NULL);

    return 0;
}