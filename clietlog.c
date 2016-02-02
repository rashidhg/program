#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/ipc.h>

pthread_t send_thread,recv_thread;

char rfifo[50],sfifo[50];

void error(char* msg)
{
	perror(msg);
	exit(1);
}

void init()
{
	int pid;
	pid = getpid();
	sprintf(rfifo,"%s%d","s_to_c",pid);
	sprintf(sfifo,"%s%d","c_to_s",pid);
	if(mkfifo(rfifo,IPC_CREAT|0666) < 0)
	error("mkfifo()");
	if(mkfifo(sfifo,IPC_CREAT|0666) < 0)
	error("mkfifo()");
	int fd = open("well_known_fifo",O_WRONLY);
	if(fd < 0)
	error("open()");
	printf("Enter the service number\n1.square\t2.Cube\n");
	int ch;
	scanf("%d",&ch);
	if(ch < 1 || ch > 2)
	error("enter valid choice");
	char buf[20];
	sprintf(buf,"%d|%d",pid,ch);
	write(fd,buf,strlen(buf));
	printf("%s\n",buf);
	close(fd);
}

void* my_send(void* args)
{
	int fd = open(sfifo,O_WRONLY);
	if(fd < 0)
	error("open()");
//	int n;
	char buf[10];
	while(1)
	{
//		scanf("%d",&n);
//		sprintf(buf,"%d",n);
		read(STDIN_FILENO,buf,10);
		write(fd,buf,strlen(buf));
	}
	close(fd);
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

void* my_recv(void* args)
{
	int n,fd = open(rfifo,O_RDONLY);
	if(fd < 0)
	error("open()");
	char buf[256];
	while(1)
	{
		n = read(fd,buf,256);
		process_buf(buf,n);
		printf("received: %s\n",buf);
	}
	close(fd);
}
int main(int argc,char* argv[])
{
	init();printf("Connected\n");
	pthread_create(&send_thread,NULL,my_send,NULL);
	pthread_create(&recv_thread,NULL,my_recv,NULL);
	pthread_join(send_thread,NULL);
	pthread_join(recv_thread,NULL);
	return 0;
}
