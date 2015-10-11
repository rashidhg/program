/*	This program is for child - parent communication through pipe	   */
/*	Here child sends the data to parent for reverse close reverse fds  */

#include <stdio.h>
#include <unistd.h>
#define M 10

int main()
{
	pid_t pid;
	char buff[M];
	int fd[2];
	pipe(fd);
	pid = fork();
		
	if(pid == 0)
	{		
		close(fd[0]);
		scanf("%s", buff);
		write(fd[1], buff, M);				
	}	
	else
	{
//		wait(NULL);
		close(fd[1]);
		read(fd[0], buff, M);
		int N = atoi(buff);
		printf("Square is %d\n", N*N);		
	}
	return 0;
}
