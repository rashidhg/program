#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<semaphore.h>
#define size 10
int buffer[size];
int pind=0,cind=0,count=0;
 sem_t semvar;
 sem_t pvar,cvar;
void * producer()
  {
    while(1)
   {   
        sleep(rand()%3); 
       sem_wait(&pvar);
       sem_wait(&semvar);
   
     
           buffer[pind]=pind;  count++;
                    
            printf("produced %d count is %d\n",buffer[pind],count);
            // printf("produced %d \n",pthread_self());
           pind=(pind+1)%size;
            
          sem_post(&semvar); sem_post(&cvar); 
   }
  }
  void * consumer()
  {
    while(1)
    {    sleep(rand()%3);
          sem_wait(&cvar);
             sem_wait(&semvar);
             count--;
              
           printf("consumed %d count is %d\n",buffer[cind],count);
            // printf("consumed %d \n",pthread_self(),count);
        cind=(cind+1)%size;
         
          
           sem_post(&semvar);  sem_post(&pvar);  
  }
  }
int main()
{
 pthread_t pid,cid;
   sem_init(&semvar,0,1);
    sem_init(&pvar,0,size);
    sem_init(&cvar,0,0);
  pthread_create(&pid,NULL,producer,NULL);
  pthread_create(&cid,NULL,consumer,NULL);
  pthread_join(pid,NULL);
  pthread_join(cid,NULL);
  exit(0);
}
