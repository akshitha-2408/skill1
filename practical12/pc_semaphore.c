#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define ITEMS_PER_PRODUCER 500000
#define BUFFER_SIZE 10
#define STOP_ITEM -1

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t empty, full;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

long long produced = 0;
long long consumed = 0;

void *producer(void *arg)
{
    long id = (long)arg;

    for (int i = 0; i < ITEMS_PER_PRODUCER; i++) {
        int item = (int)(id * ITEMS_PER_PRODUCER + i);

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        produced++;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }

    return NULL;
}

void *consumer(void *arg)
{
    (void)arg;

    while (1) {
        int item;

        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;

        if (item != STOP_ITEM)
            consumed++;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

        if (item == STOP_ITEM)
            break;
    }

    return NULL;
}

int main(void)
{
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];

    clock_t start, end;
    int total_items = NUM_PRODUCERS * ITEMS_PER_PRODUCER;

    if (sem_init(&empty, 0, BUFFER_SIZE) != 0) {
        perror("sem_init");
        return EXIT_FAILURE;
    }

    if (sem_init(&full, 0, 0) != 0) {
        perror("sem_init");
        sem_destroy(&empty);
        return EXIT_FAILURE;
    }

    start = clock();

    for (long i = 0; i < NUM_PRODUCERS; i++) {
        if (pthread_create(&producers[i], NULL, producer,
                           (void *)i) != 0) {
            perror("pthread_create");
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        if (pthread_create(&consumers[i], NULL, consumer,
                           NULL) != 0) {
            perror("pthread_create");
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < NUM_PRODUCERS; i++)
        pthread_join(producers[i], NULL);

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        buffer[in] = STOP_ITEM;
        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }

    for (int i = 0; i < NUM_CONSUMERS; i++)
        pthread_join(consumers[i], NULL);

    end = clock();

    printf("\nProducer-Consumer Experiment\n");
    printf("============================\n");
    printf("Buffer size     : %d\n", BUFFER_SIZE);
    printf("Producers       : %d\n", NUM_PRODUCERS);
    printf("Consumers       : %d\n", NUM_CONSUMERS);
    printf("Expected items  : %d\n", total_items);
    printf("Produced items  : %lld\n", produced);
    printf("Consumed items  : %lld\n", consumed);
    printf("Execution time  : %.6f seconds\n",
           (double)(end - start) / CLOCKS_PER_SEC);

    if (produced == total_items && consumed == total_items)
        printf("Synchronization : CORRECT\n");
    else
        printf("Synchronization : ERROR\n");

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
