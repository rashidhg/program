#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#define size 10
int buffer[size];
int pind=0,cind=0,count=0;
 pthread_mutex_t mvar=PTHREAD_MUTEX_INITIALIZER;
 pthread_cond_t Buff_Empty=PTHREAD_COND_INITIALIZER;
 pthread_cond_t Buff_Full=PTHREAD_COND_INITIALIZER;
void * producer()
  {
    while(1)
   {
       pthread_mutex_lock(&mvar);
     if(count==size)
      pthread_cond_wait(&Buff_Full,&mvar);
           buffer[pind]=pind;  count++;
                   // sleep(rand()%3);
            printf("produced %d count is %d\n",buffer[pind],count);
            // printf("produced %d \n",pthread_self());
           pind=(pind+1)%size;
             pthread_cond_signal(&Buff_Empty);
           pthread_mutex_unlock(&mvar);
   }
  }
  void * consumer()
  {
    while(1)
    {  pthread_mutex_lock(&mvar);
     if(count==0)
         pthread_cond_wait(&Buff_Empty,&mvar);
             count--;
               // sleep(rand()%3);
           printf("consumed %d count is %d\n",buffer[cind],count);
            // printf("consumed %d \n",pthread_self(),count);
        cind=(cind+1)%size;
         pthread_cond_signal(&Buff_Full);
           pthread_mutex_unlock(&mvar); 
  }
  }
int main()
{
 pthread_t pid,cid;
  pthread_create(&pid,NULL,producer,NULL);
  pthread_create(&cid,NULL,consumer,NULL);
  pthread_join(pid,NULL);
  pthread_join(cid,NULL);
  exit(0);
}
