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
char sfifo[20],rfifo[20];

void init(int fid)
{
	sprintf(sfifo,"%s%d","send",fid);
	sprintf(rfifo,"%s%d","rece",fid);
} 

void process_buf(char* msg,int n)
{
	int i;
	for(i=0;i<n;i++)
	{
		msg[i] = '\0';
	}
}

int main(int argc , char * argv[])
{
	int id = atoi(argv[1]);
	init(id);
	int c;
	c=fork();
	if(c==0)
	{
		int fd1 = open(sfifo,O_WRONLY);
		char buf[20];
		while(1)
		{
			fgets(buf,20,stdin);
			write(fd1,buf,strlen(buf));
			memset(buf,0,20);
		}
		close(fd1);
	}
	else
	{
		int n,fd2 = open(rfifo,O_RDONLY);
		char buf[20];
		while(1)
		{
			n=read(fd2,buf,20);
			if(n>0)
			{
				printf("%s\n",buf);
				memset(buf,0,20);
				//process_buf(buf,n);
			}
		}
		close(fd2);
	}
}
