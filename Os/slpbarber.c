#include <stdio.h>
#include <pthread.h>
#define N 10
#define C 5

pthread_mutex_t mutex;
pthread_cond_t empty;
int ncust;

void* barber(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        if(ncust == 0)
        {
            printf("barber Sleeping\n");
            pthread_cond_wait(&empty, &mutex);
        }

        sleep(rand() % 3);
        printf("Cutting Done\n");
        ncust--;
        pthread_mutex_unlock(&mutex);
    }
}

void* customer(void *arg)
{
    int id = *((int *)arg);
    pthread_mutex_lock(&mutex);
    if(ncust <= C)
    {
        ncust++;
        printf("Customer %d sat on chair\n", id);
        pthread_cond_signal(&empty);
    }
    else
    {
        printf("Customer %d left the shop\n", id);
    }
    pthread_mutex_unlock(&mutex);   
    sleep(rand() % 3);
}

int main()
{
    int id[N];
    int i;
    for (i = 0; i < N; ++i)
        id[i] = i;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&empty, NULL);
    pthread_t barber_t, customer_t[N];

    pthread_create(&barber_t, NULL, barber, NULL);
    for (i = 0; i < N; ++i)
        pthread_create(&customer_t[i], NULL, customer, (void *)&id[i]);

    pthread_join(barber_t, NULL);
    for (i = 0; i < N; ++i)
        pthread_join(customer_t[i], NULL);
    return 0;
}
