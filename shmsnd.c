#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define SHM_SIZE 100

void err(char *s)
{
	perror(s);
	exit(1);
}
/*
	shared memory and msgqueue are similar in nature with one difference
	we create msg queue at once but for shared memory we need to give
	address also after creating, i.e.  
	for msgqueue - msgget (for creation) , msgctl(for destroy) are suff.
	for shared_m - shmget, shmat(for cre.), shmctl, shmdt(for des.)

	other main difference is when we read msg  they are get deleted
	from msg queue but in shared memory they remain untill shmdt and shmctl
*/
int main()
{
	char c;
	int shmid;
	key_t key;
	char *shmptr, *s;

	if((key = ftok(".", 'A')) == -1)
		err("ftok error");
	if((shmid = shmget(key, SHM_SIZE, IPC_CREAT|0666)) < 0)
		err("shmget error");	
	if((shmptr = shmat(shmid, 0, 0)) == (char *)-1)
		err("shmat error");
/*	
	s = shmptr;
	for(c = 'a'; c <= 'z'; c++)
		*s++ = c;
*/
	char arr[] = "Utkarsh Trivedi";
	strcpy(shmptr, arr);

        printf("%s", shmptr);
	printf("data written at shared memory id : %d\n", shmid);
	return 0;
}
