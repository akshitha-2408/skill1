#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t resourceA;
pthread_mutex_t resourceB;

void *thread1(void *arg)
{
    (void)arg;

    printf("Thread 1: Trying to acquire Resource A\n");
    pthread_mutex_lock(&resourceA);
    printf("Thread 1: Acquired Resource A\n");

    sleep(1);

    printf("Thread 1: Trying to acquire Resource B\n");
    pthread_mutex_lock(&resourceB);

    printf("Thread 1: Acquired Resource B\n");
    printf("Thread 1: Using Resource A and B\n");

    pthread_mutex_unlock(&resourceB);
    pthread_mutex_unlock(&resourceA);

    printf("Thread 1: Released resources\n");
    return NULL;
}

void *thread2(void *arg)
{
    (void)arg;

    printf("Thread 2: Trying to acquire Resource B\n");
    pthread_mutex_lock(&resourceB);
    printf("Thread 2: Acquired Resource B\n");

    sleep(1);

    printf("Thread 2: Trying to acquire Resource A\n");
    pthread_mutex_lock(&resourceA);

    printf("Thread 2: Acquired Resource A\n");
    printf("Thread 2: Using Resource A and B\n");

    pthread_mutex_unlock(&resourceA);
    pthread_mutex_unlock(&resourceB);

    printf("Thread 2: Released resources\n");
    return NULL;
}

int main(void)
{
    pthread_t t1, t2;

    pthread_mutex_init(&resourceA, NULL);
    pthread_mutex_init(&resourceB, NULL);

    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&resourceA);
    pthread_mutex_destroy(&resourceB);

    printf("Program completed\n");
    return 0;
}
