#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#define M 3

sem_t rw, read;
int readerCount = 0;

void acquire_readlock()
{
    sem_wait(&read);
    readerCount++;
    if(readerCount == 1)
        sem_wait(&rw);
    sem_post(&read);
}
void release_readlock()
{
    sem_wait(&read);
    readerCount--;
    if(readerCount == 0)
        sem_post(&rw);
    sem_post(&read);
}

void acquire_writelock()
{
    sem_wait(&rw);
}
void release_writelock()
{
    sem_post(&rw);
}

void process()
{
    sleep(rand() % 3);
}

void* reader(void* arg)
{
    int id = *((int *)arg);
    while(1)
    {
        printf("Reader %d came to reading\n", id);
        acquire_readlock();
        printf("Reader %d is reading now\n", id);
        process();
        release_readlock();
        printf("Reader %d finished reading\n", id);
    }
}
void* writer(void* arg)
{
    int id = *((int *)arg);
    while(1)
    {
        printf("Writer %d came to writing\n", id);
        acquire_writelock();
        printf("Writer %d is writing now\n", id);
        process();
        release_writelock();
        printf("Writer %d finished writing\n", id);
    }
}

int main()
{
    sem_init(&rw, NULL, 1);
    sem_init(&read, NULL, 1);
    int id[M];
    int i;

    for (i = 0; i < M; ++i)
        id[i] = i + 1;

    pthread_t R[M], W[M];

    for (i = 0; i < M; ++i)
        pthread_create(&R[i], NULL, reader, &id[i]);
    
    for (i = 0; i < M; ++i)
        pthread_create(&W[i], NULL, writer, &id[i]);

    for (i = 0; i < M; ++i)
        pthread_join(R[i], NULL);

    for (i = 0; i < M; ++i)
        pthread_join(W[i], NULL);
    return 0;
}
