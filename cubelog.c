#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>

pthread_t send_thread,recv_thread;

char rfifo[50],sfifo[50];

void error(char* msg)
{
	perror(msg);
	exit(1);
}
int pid,opid;
void init()
{
	opid = (int) getpid();
	char buf[30];
	read(STDIN_FILENO,buf,30);
	pid = atoi(buf);	
	sprintf(sfifo,"%s%d","s_to_c",pid);
	sprintf(rfifo,"%s%d","c_to_s",pid);
//	if(mkfifo(rfifo,IPC_CREAT|0666) < 0)
//	error("mkfifo()");
//	if(mkfifo(sfifo,IPC_CREAT|0666) < 0)
//	error("mkfifo()");
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
	int ti,n,rfd = open(rfifo,O_RDONLY),sfd = open(sfifo,O_WRONLY);
	if(rfd < 0 || sfd < 0)
	error("open()");
	char buf[256],msg[256],tmp[256];
	sprintf(tmp,"%s%d%s%d%s","Cube service(",opid,") received from client(",pid,"): ");
	while(1)
	{
		n = read(rfd,buf,256);
		process_buf(buf,n);
		sprintf(msg,"%s%s",tmp,buf);
		ti = strlen(msg);
		msg[ti] = '\n';
		write(STDOUT_FILENO,msg,ti+1);
		n = atoi(buf);
		n = n*n*n;
		sprintf(buf,"%d",n);
		n = strlen(buf);
		buf[n] = '\n';
		write(sfd,buf,n+1);
	}
	close(sfd);
	close(rfd);
}

int main(int argc,char* argv[])
{
	init();
	recv();
	return 0;
}
