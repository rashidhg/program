#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>


char fifo[50],sfifo[50],num[20];
int ffd;

void error(char* msg)
{
	printf("%s\n",msg);
}

void init()
{
	int pid;
	char buf[30];
	read(STDIN_FILENO,buf,30);
	pid = atoi(buf);	
	sprintf(fifo,"%s%d","fifo",pid);
	sprintf(sfifo,"square_fifo");
	ffd = open(sfifo,O_RDONLY|O_NONBLOCK);
}

void process_buf(char* msg,int n)
{
	int i;
	for(i=0;i<n;i++)
	if(msg[i] == '\n')
	{
		msg[i] = '\0';	
		break;
	}
}

void recv()
{
	int n,n1,fd = open(fifo,O_WRONLY);
	char buf[256];
	n = read(ffd,num,20*sizeof(char));
	close(ffd);
	process_buf(num,n);
	n = atoi(num);
	n1 = n*n;
	sprintf(buf,"square of %d = %d|",n,n1);
	n = strlen(buf);
	buf[n] = '\n';
	write(fd,buf,n+1);
	close(fd);
}

int main(int argc,char* argv[])
{
	init();
	recv();
	return 0;
}
