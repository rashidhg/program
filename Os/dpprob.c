#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#define n 20
#define nofeats 10
pthread_mutex_t spoons[n];
int a[n];
void * philosopher(void *param)
  {
     int x=*((int *)param);  
	 int i;
  for(i=0;i<nofeats;i++)  
    {
    while(a[x]!=0||a[(x+1)%n]!=0);
     a[x]=1; a[(x+1)%n]=1;
    pthread_mutex_lock(&spoons[x]);
    pthread_mutex_lock(&spoons[(x+1)%n]);
     printf("philosopher %d eating\n",x);
    sleep(rand()%2);
     printf("philosopher %d thinking\n",x);
      a[x]=0; a[(x+1)%n]=0;
    pthread_mutex_unlock(&spoons[x]);
    pthread_mutex_unlock(&spoons[(x+1)%n]);
    sleep(rand()%3);
   }
      printf("philosopher %d eating done\n",x);
    
  }
int main()
{
  pthread_t phil[n];
   int i;
   
   for(i=0;i<n;i++)
{
   pthread_mutex_init(&spoons[i],NULL);  a[i]=0;
}
   for(i=0;i<n;i++)
    {
      int *pn=malloc(sizeof(int));
      *pn=i;
    pthread_create(&phil[i],NULL,philosopher,pn);
    
    }
    for(i=0;i<n;i++)
    pthread_join(phil[i],NULL);
  exit(0);
}
