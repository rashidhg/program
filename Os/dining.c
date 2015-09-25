#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#define P 5

pthread_mutex_t mutex[P], lock;

void think(int id)
{
    printf("Philospher %d is thinking\n", id);
    sleep(rand() % 3);
}
void eat(int id)
{
    printf("Philospher %d started eating\n", id);
    sleep(rand() % 3);
    printf("Philospher %d finished eating\n", id);
    
    pthread_mutex_unlock(&mutex[id]);
    pthread_mutex_unlock(&mutex[(id + 1) % P]);
}

int hungry(int id)
{
	pthread_mutex_lock(&lock);
	pthread_mutex_lock(&mutex[(id + 1) % P]);
	printf("Philospher %d acquired right fork\n", id);
	pthread_mutex_unlock(&lock);
	
	pthread_mutex_lock(&lock);
	if(pthread_mutex_trylock(&mutex[id]) == 0)				//	return 0 means lock is acquired
	{
	    printf("Philospher %d acquired left fork\n", id);   		
	    pthread_mutex_unlock(&lock);
	   	return 1;
	}
	else
	{
		pthread_mutex_unlock(&mutex[(id + 1) % P]);
		printf("Philospher %d released right fork\n", id);	
	    pthread_mutex_unlock(&lock);
		return 0;
	}
}

void* process(void *arg)
{
    int id = *((int *)arg);
    while(1)
    {
        think(id);
        if(hungry(id))
	        eat(id);
    }
}

int main()
{
    int id[P];
    int i;
    for (i = 0; i < P; ++i)
        id[i] = i;

    for (i = 0; i < P; ++i)
        pthread_mutex_init(&mutex[i], NULL);
    pthread_mutex_init(&lock, NULL);
    
    pthread_t philospher[P];
    for (i = 0; i < P; ++i)
        pthread_create(&philospher[i], NULL, process, (void *)&id[i]);

    for (i = 0; i < P; ++i)
        pthread_join(philospher[i], NULL);

    return 0;
}
