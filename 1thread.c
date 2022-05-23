#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <sys/time.h>

#define NUM_THREADS 1



void*
Work(void *t)
{
    int i;
    long tid;
    double result=0.0;

    tid = (long)t;
    printf("Thread %ld starting...\n",tid);

    for (i = 0; i < 400000000; i++)
    {
        result += 1.0;
    }

    printf("Thread %ld done. Result = %f\n",tid, result);
    pthread_exit((void*) t);
}

void* 
PthreadCreate(void* arg)
{
    pthread_t thread[NUM_THREADS];
    pthread_attr_t attr;
    int rc;
    long t;
    void *status;

    struct timeval start_point, end_point;
    double operating_time;

    gettimeofday(&start_point, NULL);


    /* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for(t = 0; t < NUM_THREADS; t++) 
    {
        printf("Main: creating thread %ld\n", t);
        rc = pthread_create(&thread[t], &attr, Work, (void *)t); 
        if (rc) 
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            return NULL;
        }
    }

    /* Free attribute and wait for the other threads */
    pthread_attr_destroy(&attr);
    for(t = 0; t < NUM_THREADS; t++) 
    {
        rc = pthread_join(thread[t], &status);
        if (rc) 
        {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            return NULL;
        }
        printf("Main: completed join with thread %ld having a status of %ld\n",t,(long)status);
    }

    printf("Main: program completed. Exiting.\n");

    gettimeofday(&end_point, NULL); 
    operating_time = (double)(end_point.tv_sec)+(double)(end_point.tv_usec)/1000000.0-(double)(start_point.tv_sec)-(double)(start_point.tv_usec)/1000000.0;
    printf("Toal elapsed time = %f(sec)\n",operating_time);

    pthread_exit(NULL);
}

int
main(void)
{
    pthread_t mainThread;

    pthread_create(&mainThread, NULL, PthreadCreate, NULL);
    pthread_exit(NULL);

    return 0;
}
