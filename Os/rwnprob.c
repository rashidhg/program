#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<semaphore.h>
# define nr 20
# define nw 5
sem_t r,w,mutex;
int rcount=0;
void * readit(void * param)
{
  int x=*((int *)param);  int t;

   do
  {
   sem_getvalue(&w,&t);
  }while(t==0);
 
     rcount++;
    sem_post(&mutex);
    printf("reading..%d\n",x);
   // sleep(rand()%4);
   printf("reading..done %d\n",x);
    sem_wait(&mutex);
     rcount--;
    sem_post(&mutex);
}
void * writeit(void * param)
{  int x=*((int *)param); 
     
    sem_wait(&w);
      while(rcount!=0);
     
     printf("writing..%d\n",x);
    //sleep(rand()%3);
   printf("writing..done %d\n",x);
    sem_post(&w);
}
int main()
{
  pthread_t pr[nr],pw[nw];  int i,t;
   sem_init(&mutex,0,1); 
   sem_init(&r,0,1);
   sem_init(&w,0,1);
   
  if(nr<nw) t=nr; else t=nw;
   for(i=0;i<t;i++)
   {
      int *pn=malloc(sizeof(int));
      *pn=i;
   pthread_create(&pr[i],NULL,readit,(void *)pn); 
   pthread_create(&pw[i],NULL,writeit,(void *)pn);
   }
    for(;i<nr;i++)  {
       int *pn=malloc(sizeof(int));
      *pn=i;
     pthread_create(&pr[i],NULL,readit,(void *)pn); 
    }
    for(;i<nw;i++){
      int *pn=malloc(sizeof(int));
      *pn=i;
     pthread_create(&pw[i],NULL,writeit,(void *)pn); 
    }
for(i=0;i<t;i++)
{
    pthread_join(pr[i],NULL);
   pthread_join(pw[i],NULL);
}
   for(;i<nr;i++)   pthread_join(pr[i],NULL);
    for(;i<nw;i++)  pthread_join(pw[i],NULL);
  exit(0);
}

