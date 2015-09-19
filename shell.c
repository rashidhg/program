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
    const char delim[2] = " ";
    char dirnIn = NULL;
    char dirnOut = NULL;
    int daemon;
    daemon = 0;
    while(1)
	{
        printf("> ");
        fgets(com,100,stdin);
        if(strcmp(com,"exit") == 0)
            break;
        paraCount = 0;
        char * token = strtok(com,delim);
        while(token != NULL){
            strcpy(para[paraCount++],token);
            token = strtok(NULL,delim);
        }
        para[paraCount-1][strlen(para[paraCount-1])-1] = '\0';
        int i;
        char * command = para[0];
        char * arg = NULL;

        for(i = 0;i<paraCount;i++){
            if(para[i][0] == '-')
                arg = para[i];
            else if(para[i][0] == '&')
                daemon = 1;
            else if(para[i][0] == '<'){
                dirnIn = '<';
                fileNameIn = para[i+1];
            }
            else if(para[i][0] == '>'){
                dirnOut = '>';
                fileNameOut = para[i + 1];
            }
        }

        //forking
        pid = fork();
        if(pid == 0){
            if(daemon == 1)
            {
                setsid();
                umask(0);
            }
            int in = NULL;
            int out = NULL;


            if(dirnOut == '>'){
                out = open(fileNameOut,O_CREAT | O_WRONLY);
                dup2(out,STDOUT_FILENO);
            }
            if(dirnIn == '<'){
                in = open(fileNameIn,O_RDONLY);
                dup2(in,STDIN_FILENO);
            }


            if(execl(command,command,arg,NULL) == -1)
                exit(EXIT_FAILURE);
        }
        else{ //parent process
            if(daemon == 0){
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
