#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>

void error(char* msg)
{
	perror(msg);
	exit(1);
}

pthread_t recv_thread,send_thread;
int shmid;
char send_fifo[20],recv_fifo[20];
struct client_info
{
	int num_clients;
	int avail_id[10];
};
struct client_info *clients;
void init()
{
	key_t key = ftok(".",1234);
	if((shmid = shmget(key,sizeof(struct client_info),IPC_CREAT|0666)) < 0)
	error("shmget()");
	clients = (struct client_info*) shmat(shmid,NULL,0);
	int i;
	for(i=0;i<10;i++)
	if(clients[0].avail_id[i] == 1)
	{
		clients[0].avail_id[i] = 0;
		break;
	}
	clients[0].num_clients++;	
	sprintf(send_fifo,"%s%d","c_to_s_fifo",i);//client id
	sprintf(recv_fifo,"%s%d","s_to_c_fifo",i);
//	n[0]++;
	shmdt(clients);
	printf("%s : %s\n",send_fifo,recv_fifo);
}

void* send_msg(void* args)
{
	int fd = open(send_fifo,O_WRONLY);
	char buf[256];
	if(fd < 0)
	error("open()");
	while(1)
	{
		fgets(buf,256,stdin);
		write(fd,buf,strlen(buf));
	}
	close(fd);
}

void* recv_msg(void* args)
{
	int fd = open(recv_fifo,O_RDONLY);
	char buf[256];
	if(fd < 0)	
	error("open()");
	while(1)
	{
		read(fd,buf,256);
		printf("%s\n",buf);
	}
	close(fd);
}
		
int main(int argc,char* argv[])
{
	init();
	pthread_create(&send_thread,NULL,send_msg,NULL);
	pthread_create(&recv_thread,NULL,recv_msg,NULL);
	pthread_join(send_thread,NULL);
	pthread_join(recv_thread,NULL);	
	return 0;
}
