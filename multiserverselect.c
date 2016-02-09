#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <string.h>
#include <fcntl.h>

void error(char *msg)
{
	printf("%s\n",msg);
}
char fifo[20];

fd_set rfds;
struct timeval tv;
int arr_fd[20];
int fd_main;
void init()
{

	mkfifo("main",IPC_CREAT|0666);
	fd_main = open("main",O_RDONLY|O_NONBLOCK);
	mkfifo("square_fifo",IPC_CREAT|0666);
	mkfifo("cube_fifo",IPC_CREAT|0666);
	
}

void process_buf(char* msg,int n,char* pid,char* service_id,char* num)
{
	int i;char *t;
	for(i=0;i<n;i++)
	{
		if(msg[i] == '\n')
		{
			msg[i] = '\0';
			break;
		}	
	}
	strcpy(pid, strtok(msg,"|"));
	strcpy(service_id, strtok(NULL,"|"));
	strcpy(num, strtok(NULL,"|"));
}

int main(int argc , char * argv[])
{
	init();
	int res,n,c,fd;
	char pid[20],choice[20],buf[128],num[20];
	while(1)
	{
		FD_ZERO(&rfds);
		for(i=0;i<n;i++)
		{
    		FD_SET(arr_fd[i], &rfds);
		}
    	tv.tv_sec = 3;
    	tv.tv_usec = 0;
    	int retval = select(fd_main+1, &rfds, NULL, NULL, &tv);
		if(retval > 0 && FD_ISSET(fd_main,&rfds))
		{
			n = read(fd_main,buf,128);
			process_buf(buf,n,pid,choice,num);
			sprintf(buf,"%s_fifo",argv[atoi(choice)]);
			fd = open(buf,O_WRONLY);
			write(fd,num,strlen(num));
			close(fd);
			int pp[2];
			pipe(pp);
			c = fork();
			if(c>0)
			{
				close(pp[0]);
				write(pp[1],pid,strlen(pid));
				close(pp[1]);
			}
			else if(c==0)
			{
				close(pp[1]);
				dup2(pp[0],0);
				close(pp[0]);
				execl(argv[atoi(choice)],argv[atoi(choice)],NULL);
			}
			else
				error("fork");
		}
	}
	
}
