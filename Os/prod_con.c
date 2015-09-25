#include <stdio.h>
#include <pthread.h>
#define M 10
#define C 3

pthread_mutex_t mutex;
pthread_cond_t cond, condC;
int item, cnt;
int shared[M];
int head, tail;

void* Producer(void *arg)
{
	while(1)
	{
	    pthread_mutex_lock(&mutex);
	    while(cnt >= M) 
	        pthread_cond_wait(&cond, &mutex);
	    shared[tail] = item;
	    tail = (tail + 1) % M;
	    cnt++;
	    printf("Produced item %d\n", item);
	    item++;
	    pthread_mutex_unlock(&mutex);
	    pthread_cond_broadcast(&condC);
	    sleep(rand() % 2);
	}
}   

void* Consumer(void *arg)
{
    int id = *((int *)arg);
	while(1)
	{
		pthread_mutex_lock(&mutex);
	    while(cnt <= 0)
	        pthread_cond_wait(&condC, &mutex);
	    int op = shared[head];
	    head = (head + 1) % M;
	    cnt--;
	    printf("Consumed item no %d by consumer no %d\n", op, id);
	    pthread_mutex_unlock(&mutex);
	    pthread_cond_signal(&cond);
	    sleep(rand() % 3);
	}
}

int main()
{
    int id[3] = {1, 2, 3};
    int i;
    item = 1;
    head = tail = cnt = 0;
    pthread_cond_init(&cond, NULL);
    pthread_cond_init(&condC, NULL);

    pthread_mutex_init(&mutex, NULL);
    pthread_t p, c[C];

    pthread_create(&p, NULL, Producer, NULL);
    for (i = 0; i < C; ++i)
        pthread_create(&c[i], NULL, Consumer, &id[i]);
    
    pthread_join(p, NULL);
    for (i = 0; i < C; ++i)
        pthread_join(c[i], NULL);
    
    return 0;
}
