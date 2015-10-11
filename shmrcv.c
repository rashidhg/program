#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void err(char* s)
{
	perror(s);
	exit(1);
}

int main(int argc, char** argv)
{
	int shmid;
	char *shm, *s;
	char c;

	if(argc < 2)
		err("Insufficient argument");

	shmid = atoi(argv[1]);	
	if((shm = shmat(shmid, NULL, 0)) == (char*)-1)	
		err("shmat error");	
	
	printf("Reading from shared Memory :\n");
//	printf("%s", shm);

	for(s = shm; *s != '\0'; s++)
		putchar(*s);
	putchar('\n');

	printf("Do you want to remove Shared mem (y/n) :");
	c = getchar();
	if(c == 'y')
	{
		if(shmdt(shm) == -1)
			err("shmdt");
		if((shmctl(shmid, IPC_RMID, NULL)) == -1)
			err("shmctl");
	}
	return 0;
}
