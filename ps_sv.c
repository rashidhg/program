#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#define SH_SIZE 10
#define P 3
#define C 3

struct sembuf sopslock;
struct sembuf sopsunlock;

struct Shared
{
	int buf[SH_SIZE];
	int in, out, full, empty, mutex;
};
struct Shared shared;
int item = 0;

void err(char *s)
{
	perror(s);
	exit(1);
}

void sem_acquire(int semid)
{
	if(semop(semid, &sopslock, 1) == -1)
		err("sem_acquire");
}

void sem_release(int semid)
{
	if(semop(semid, &sopsunlock, 1) == -1)
		err("sem_release");
}

void producer(void* arg)
{
	int id;
	id = *(int *)arg;
	printf("Producer %d\n", id);

	while(1)
	{
		sem_acquire(shared.empty);
		sem_acquire(shared.mutex);
		shared.buf[shared.in] = item++;
		shared.in = (shared.in + 1) % SH_SIZE;
		printf("Producer %d produced item no : %d\n", id, item);
		sem_release(shared.mutex);
		sem_release(shared.full);
		sleep(rand() % 3);
	}
}

void consumer(void* arg)
{
	int itm, id;
	id = *(int *)arg;
	printf("Consumer %d\n", id);
	while(1)
	{
		sem_acquire(shared.full);
		sem_acquire(shared.mutex);
		itm = shared.buf[shared.out];
		shared.out = (shared.out + 1) % SH_SIZE;
		printf("Consumer %d consumed item no %d\n", id, itm);
		sem_release(shared.mutex);
		sem_release(shared.empty);
		sleep(rand() % 3);
	}	
}

int main()
{
	shared.in = shared.out = 0;
	sopslock.sem_num = 0;
	sopslock.sem_op = -1;
	sopslock.sem_flg = 0;

	sopsunlock.sem_num = 0;
	sopsunlock.sem_op = 1;
	sopsunlock.sem_flg = 0;
	
	if((shared.full = semget(1234, 1, 0666|IPC_CREAT)) == -1)
		err("semget full");
	semctl(shared.full, 0, SETVAL, 0);
      
	if((shared.empty = semget(5678, 1, 0666|IPC_CREAT)) == -1)
		err("semget empty");
	semctl(shared.empty, 0, SETVAL, SH_SIZE);

	if((shared.mutex = semget(9012, 1, 0666|IPC_CREAT)) == -1)
		err("semget mutex");
	semctl(shared.mutex, 0, SETVAL, 1);

	pthread_t p[P], c[C];
	int i, id[C];

	for(i = 0; i < C; i++)
		id[i] = i + 1;

	for(i = 0; i < P; i++)
		pthread_create(&p[i], NULL, producer, &id[i]);	
	for(i = 0; i < C; i++)
		pthread_create(&c[i], NULL, consumer, &id[i]);

	for(i = 0; i < P; i++)
		pthread_join(&p[i], NULL);
	for(i = 0; i < C; i++)
		pthread_join(&c[i], NULL);

	return 0;
}
