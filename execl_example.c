#include <stdio.h>
#include <unistd.h>

int main()
{
	int pid;
	pid = fork();
	if(pid == 0)
	{
		execl("/bin/ls", "/home/utkarsh/Prgrams", NULL);
		printf("Won't print\n");
	}
//	else
//		printf("Parent Process\n");
}
