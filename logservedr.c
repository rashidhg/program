#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

void error(char* msg)
{
	perror(msg);
	exit(1);
}
int ffd;
struct pollfd* fds;
struct pollfd log_fd[3];
int num_log;
void init()
{
	if(mkfifo("well_known_fifo",IPC_CREAT|0666) < 0)
	error("mkfifo()");
	ffd = open("well_known_fifo",O_RDONLY);//|O_NONBLOCK);
	if(ffd < 0)
	error("open()");
	fds = (struct pollfd*) malloc(sizeof(struct pollfd));
	fds->fd = ffd;
	fds->events = POLLIN;
	num_log = 0;
}

void process_buf1(char* msg,int n)
{
	int i;	
	for(i=0;i<n;i++)
	if(msg[i] == '\n')
	{
		msg[i] = '\0';
		break;
	}
}

void process_buf(char* msg,int n,char* pid,char* service_id)
{
	int i;
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
//	printf("PPP : %s %s\n",pid,service_id);
//	printf("%d\n",(int)strlen(pid));
//	if(pid[strlen(pid)] == '\0')	printf("yes");
//	scanf("%s",pid);
//	printf("%d\n",(int)strlen(pid));
//	if(pid[strlen(pid)-1] == '\0')printf("Yes");
}

int main(int argc,char* argv[])
{
	
	int i,n;
	if(argc < 2)
	{
		printf("Usage: %s Service names",argv[0]);
		exit(1);
	}
	init();
	char buf[256],pid[128],service_id[128];
//	strcpy(buf,"1234|Jayesh");process_buf(buf,strlen(buf),pid,service_id);return 0;
	
	while(1)
	{	
	n= poll(fds,1,0);
	if(n > 0 && (fds->revents & POLLIN))
	{	n = read(ffd,buf,256);
		process_buf(buf,n,pid,service_id);
//		printf("YYY : %s %s %s %s\n",buf,pid,service_id,argv[atoi(service_id)]);
		int mss_to_service_fd[2],service_to_mss_fd[2];
		if(pipe(mss_to_service_fd) < 0)
		error("pipe()");
		if(pipe(service_to_mss_fd) < 0)
		error("pipe()");
		pid_t cpid;
		cpid = fork();
		if(cpid > 0)
		{
			close(mss_to_service_fd[0]);
			write(mss_to_service_fd[1],pid,strlen(pid));
			close(mss_to_service_fd[1]);
			log_fd[num_log].fd = service_to_mss_fd[0];
			log_fd[num_log].events = POLLIN;
			num_log++;
			close(service_to_mss_fd[1]);
		}
		else if(cpid == 0)
		{
			close(service_to_mss_fd[0]);
			dup2(service_to_mss_fd[1],1);
			close(service_to_mss_fd[1]);
		
			close(mss_to_service_fd[1]);
			dup2(mss_to_service_fd[0],0);
			close(mss_to_service_fd[0]);
			execl(argv[atoi(service_id)],argv[atoi(service_id)],NULL);
		}
		else
		error("fork()");
//		sleep(2);
	}
	if(num_log > 0)//services are running
	{
		n = poll(log_fd,num_log,0);
		if(n > 0)
		{
			for(i=0;i<num_log;i++)
			{
				if(log_fd[i].revents & POLLIN)
				{
					n = read(log_fd[i].fd,buf,256);
					process_buf1(buf,n);
					printf("%s\n",buf);	
				}
			}
		}
	}
	}		 
	return 0;
}

