#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define M 100

struct my_msg
{
	long mtype;
	char text[M];
};

int main(int argc, char** argv)
{
	struct my_msg msg;
	int msqid;
	key_t key;
	if(argc < 2)
	{
		printf("Please give msqid as arg\n");
		exit(1);
	}

	/*
	if((key = ftok("msgsnd.c", 'B')) == -1)
	{
		perror("ftok");
		exit(1);
	}
	*/
        
	/*
	if((msqid = msgget(key, 0666)) == -1)
	{
		perror("msgget");
		exit(1);
	}
	*/
	
	msqid = atoi(argv[1]);
	struct msqid_ds qbuf;
	if(msgctl(msqid, IPC_STAT, &qbuf) == -1)	//   copy msg queue
	{
		perror("msgctl");
		exit(1);
	}
	int msgcnt = qbuf.msg_qnum;
	
	printf("Receiving msgs from %d queue ", msqid);	
	while(msgcnt--)
	{
		msgrcv(msqid, &msg, sizeof(msg.text), 0, 0);
		printf("-> %s\n", msg.text);
	}

	printf("Now deleted msg_queue after reading msgs\n");
	if((msgctl(msqid, IPC_RMID, NULL)) == -1)
	{
		perror("msgctl");
		exit(1);
	}

	return 0;
}
