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
	printf("error : %s\n",msg);
}

char sfifo[20],rfifo[20];

fd_set rfds;
struct timeval tv;
int arr_fd[20];
int max;
void init(int n)
{
	int i;
	max=0;
	for(i=0;i<n;i++)
	{
		sprintf(sfifo,"%s%d","send",i+1);
		sprintf(rfifo,"%s%d","rece",i+1);
		mkfifo(sfifo,IPC_CREAT|0666);
		mkfifo(rfifo,IPC_CREAT|0666);
		arr_fd[i] = open(sfifo,O_RDONLY);
		if(max < arr_fd[i])
			max = arr_fd[i];
	}
}

void check_fd(int *fd,int n)
{
	int i;
	char buf[20];
	for(i=0;i<n;i++)
	{
		sprintf(buf,"%s%d","rece",i+1);
		fd[i] = open(buf,O_WRONLY);
		if(fd[i] < 0)
		error(" open()");
	}
}

void process_buf(char* msg,int n)
{
	int i;
	for(i=0;i<n;i++)
	{
		msg[i] ='\0';
	}
}

int main(int argv , char * argc[])
{
	if(argv != 2)
		error("arguments");
	int n=atoi(argc[1]);
	int fd[n],i,j,res,n1;
	init(n);
	check_fd(fd,n);
	while(1)
	{
		FD_ZERO(&rfds);
		for(i=0;i<n;i++)
		{
    		FD_SET(arr_fd[i], &rfds);
		}
    	tv.tv_sec = 3;
    	tv.tv_usec = 0;
    	int retval = select(max+1, &rfds, NULL, NULL, &tv);
    	if(retval > 0)
    	{
    		for(i=0;i<n;i++)
    		{
    			if(FD_ISSET(arr_fd[i],&rfds))
    			{
    				char buf[20],msg[30];
    				n1=read(arr_fd[i],buf,20*sizeof(char));
					printf("client%d's msg:%s\n",i+1,buf);
					sprintf(msg,"%s%d%s%s","client ",i+1,":",buf);
					memset(buf,0,20);
					for(j=0;j<n;j++)
					{
						if(i!=j)
						{
							write(fd[j],msg,strlen(msg));
						}
					}
					memset(msg,0,20);			
    			}
    		}
    	}
	}
}
