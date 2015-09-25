#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#define S 3

pthread_mutex_t mutex;
pthread_cond_t canMake, canSmoke;
int notCreate;
int no;

void* agent(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        while(no != 0)
			pthread_cond_wait(&canMake, &mutex);
			
        notCreate = rand() % 3;
        printf("Agent not created : %d\n", notCreate + 1);
        no = 1;
        sleep(rand() % 3);  
        pthread_mutex_unlock(&mutex);
        pthread_cond_broadcast(&canSmoke);
    }
}

void* smoker(void *arg)
{
    int id = *((int *)arg);
    while(1)
    {
        pthread_mutex_lock(&mutex);
        while(no == 0)
            pthread_cond_wait(&canSmoke, &mutex);
		printf("smoker %d is trying \n", id + 1);        
        if(id == notCreate)
        {
            printf("smoker %d is smoking now\n", id + 1);
            sleep(rand() % 3);
            no = 0;
        }
        pthread_mutex_unlock(&mutex);
//		if(no == 0)
		pthread_cond_signal(&canMake);
	}
}

int main()
{
    int id[S];
    int i;
    for (i = 0; i < S; ++i)
        id[i] = i;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&canMake, NULL);
    pthread_cond_init(&canSmoke, NULL);
    pthread_t agent_t, smoker_t[S];

    pthread_create(&agent_t, NULL, agent, NULL);
    for (i = 0; i < S; ++i)
        pthread_create(&smoker_t[i], NULL, smoker, (void *)&id[i]);

    pthread_join(agent_t, NULL);
    for (i = 0; i < S; ++i)
        pthread_join(smoker_t[i], NULL);
    return 0;
}
