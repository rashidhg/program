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
int f1,f2;
void init()
{
	int pid = getpid();
	sprintf(fifo,"%s%d","fifo",pid);
	if(mkfifo(fifo,IPC_CREAT|0666)<0)
	error("mkfifo");
	int fd = open("main",O_WRONLY);
	if(fd<0)
	error("open");
	printf("Enter the service number\n1.square\t2.Cube\n");
    int ch,n;
    scanf("%d",&ch);
    if(ch < 1 || ch > 2)
	error("enter valid choice");
	printf("Enter NUMBER : ");
	scanf("%d",&n);
    char buf[20];
    sprintf(buf,"%d|%d|%d|",pid,ch,n);
    //printf("%s\n",buf);
    int res = write(fd,buf,strlen(buf));
    //printf("%d\n",res);
    close(fd);
    f1 = open("square_fifo",O_RDONLY|O_NONBLOCK);
	f2 = open("cube_fifo",O_RDONLY|O_NONBLOCK);
	//printf("###\n");
}

void process_buf(char* msg,int n)
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
	strcpy(msg,strtok(msg,"|"));
}

int main(int argc , char * argv[])
{
	init();
	int n,fdo;
	char buf[256];
	fdo = open(fifo,O_RDONLY);
	while(1)
	{
		n = read(fdo,buf,256*sizeof(char));
		if(n > 0)
			break;
	}
	process_buf(buf,n);
	printf("%s\n",buf);
}
