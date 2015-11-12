#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
void main(int argc,char ** argv)
{
    pid_t pid;
    char para[5][20];
    char com[100];

    int paraCount;
    char* fileNameOut = NULL;
    char* fileNameIn = NULL;
    char* pipein = NULL;
    char* pipeout = NULL; 
    const char delim[2] = " ";
    char dirnIn = NULL;
    char dirnOut = NULL;
    int daemon = 0;
    
    while(1)
    {
        printf(">> ");
        fgets(com,100,stdin);
        if(strcmp(com,"exit\n") == 0)
            break;
        paraCount = 0;
        char * token = strtok(com,delim);
        while(token != NULL)
        {
            strcpy(para[paraCount++],token);
            token = strtok(NULL,delim);
        }

        para[paraCount-1][strlen(para[paraCount-1])-1] = '\0';
        int i;
        char * command = para[0];
        char * arg = NULL;
	int pipe = 0;		

        for(i = 0;i<paraCount;i++)
	{
            if(para[i][0] == '-')
                arg = para[i];
            else if(para[i][0] == '&')
                daemon = 1;

	    else if(para[i][0] == '|')	
	    {
		pipe = 1;
		pipein = para[i-1];
		pipeout = para[i+1];
            }
	    else if(para[i][0] == '<')
	    {
                dirnIn = '<';
                fileNameIn = para[i+1];
            }
            else if(para[i][0] == '>')
            {
                dirnOut = '>';
                fileNameOut = para[i + 1];
            }
        }

        //forking

        if((pid = fork()) == -1)
		printf("Fork Error\n");

        if(pid == 0)
	{
	    printf("Here Now 1");
            if(daemon == 1)
            {
		printf("Here Now 2");
                setsid();
                umask(0);
            }
            int in = NULL;
            int out = NULL;
	   
	    printf("Here Now 3");
            
            if(pipe == 1)
	    {
		printf("Here");
//		int dummy = open("dummy.txt", O_CREAT | O_RDWR);             can't access same variable
		
//		int stdoutCopy = dup(STDOUT_FILENO);

/*	
	statement after execl doesn't execute because whole address space of process is replaced by given command's address space    
	so if you want to do something after execl (as below : after first command second one should execute)  so fork the process
	and do the second thing in child process after wait (so that first process may end)
*/

		if(fork())
		{
			int dummy = open("dummy.txt", O_CREAT | O_WRONLY);
			dup2(dummy, STDOUT_FILENO);
			execl(pipein, pipein, NULL, NULL);
//			dup2(stdoutCopy, STDOUT_FILENO);
		}
		else
		{
			wait(NULL);
			int dummy = open("dummy.txt", O_RDONLY);
			dup2(dummy, STDIN_FILENO);
			execl(pipeout, pipeout, NULL, NULL);			
		}
		close(dummy);
	    }
	    else
	    {
		if(dirnOut == '>')
		{
		    out = open(fileNameOut, O_CREAT | O_WRONLY);
		    dup2(out, STDOUT_FILENO);
		}
		if(dirnIn == '<')
		{
		    in = open(fileNameIn, O_RDONLY);
		    dup2(in, STDIN_FILENO);
		}
		printf("Before execl\n");
		if(execl(command, command, arg, NULL) == -1)
		    exit(EXIT_FAILURE);
	    }        
	}
        else               //parent process
	{ 
            if(daemon == 0)
	    {
                int status;
                wait(&status);
                if(status == 0)
                    printf("\nSucessfully executed..\n");
                else
                    printf("\nError in execution..\n");
            }
            else
                printf("Task running in the background\n");
        }

        printf("\n");
    }
}
