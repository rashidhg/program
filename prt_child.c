#include <stdio.h>
#include <sys/types.h>

int main()
{
	int pid;
	pid = fork();	

	if(pid == 0)
	{
		printf("In child Process :\n");
		printf("PID : %d , Parent's PID : %d\n", getpid(), getppid());
	}
	else
	{
		sleep(10);
		printf("In parent Process :\n");
		printf("PID : %d , Child's PID : %d\n", getpid(), pid);		
	}

}
