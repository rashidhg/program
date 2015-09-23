#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#define chairs 10
#define n 50
void * barber();
void * customer();
int ncust=0;

pthread_mutex_t mvar=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t shop_empty=PTHREAD_COND_INITIALIZER;

int main()
{
 pthread_t btid,ctid[n];  int i;
   pthread_create(&btid,NULL,barber,NULL);
   for(i=0;i<n;i++)
   {
     int *pn=malloc(sizeof(int));
      *pn=i;
    pthread_create(&ctid[i],NULL,customer,(void *)pn);
   }
  pthread_join(btid,NULL);
   for(i=0;i<n;i++)
    pthread_join(ctid[i],NULL);
}
void * customer(void *param)
{
  int x=*((int *)param);  
  pthread_mutex_lock(&mvar);
   if(ncust!=chairs)
    {
         printf("customer %d added \n",x);
        ncust++;   
       pthread_cond_signal(&shop_empty);
       pthread_mutex_unlock(&mvar);     
    }
     else    { printf("customer %d left as shop is full\n",x);
               pthread_mutex_unlock(&mvar);}
    sleep(rand()%3);
}
void * barber()
{
   while(1)
  {
  pthread_mutex_lock(&mvar);
  if(ncust==0)
   {
      printf("barber sleeping\n");
     pthread_cond_wait(&shop_empty,&mvar);
     }
     sleep(rand()%3);
    printf("cutting done..\n");
   ncust--;
     
     pthread_mutex_unlock(&mvar);
  }
}
