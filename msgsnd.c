#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define M 100

struct mymsg
{
	long mtype;
	char text[M];
};

int main()
{
	struct mymsg msg;
	int msqid;
	key_t key;
	
	if((key = ftok("msgsnd.c", 'B')) == -1)
	{
		perror("ftok");
		exit(1);
	}

	if((msqid = msgget(key, 0666 | IPC_CREAT)) == -1)	
	{
		perror("msgget");
		exit(1);
	}		

	printf("Enter Text : (Enter exit at the end)\n");
	while(fgets(msg.text, sizeof(msg.text), stdin) != NULL)
	{
		if(strcmp(msg.text, "exit\n") == 0)
			break;

		int len = strlen(msg.text);
		if(msg.text[len - 1] == '\n')	
			msg.text[len - 1] = '\0';
		if((msgsnd(msqid, &msg, len + 1, 0)) == -1)
			perror("msgsnd");
	}

	//	deletes the msg_queue
	
	/*	
	if(msgctl(msqid, IPC_RMID, NULL) == -1)
	{
		perror("msgctl");
		exit(1);
	}
	*/

	printf("Now receive msg from msqid - %d\n", msqid);
	return 0;
}
