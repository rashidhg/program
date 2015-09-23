#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
# define nr 10
# define nw 5
pthread_rwlock_t rwlock=PTHREAD_RWLOCK_INITIALIZER;
void * readit()
{
  pthread_rwlock_rdlock(&rwlock);
   printf("reading x=%d\n",x);
  pthread_rwlock_unlock(&rwlock);
}
void * writeit()
{
  pthread_rwlock_wrlock(&rwlock);
    x++;
   printf("writing x=%d\n",x);
  pthread_rwlock_unlock(&rwlock);
}
int main()
{
  pthread_t pr[nr],pw[nw];  int i,t;
  if(nr<nw) t=nr; else t=nw;
   for(i=0;i<t;i++)
   {
   pthread_create(&pr[i],NULL,readit,NULL); 
   pthread_create(&pw[i],NULL,writeit,NULL);
   }
    for(;i<nr;i++)  pthread_create(&pr[i],NULL,readit,NULL); 
    for(;i<nw;i++)  pthread_create(&pw[i],NULL,writeit,NULL); 
for(i=0;i<t;i++)
{
    pthread_join(pr[i],NULL);
   pthread_join(pw[i],NULL);
}
   for(;i<nr;i++)   pthread_join(pr[i],NULL);
    for(;i<nw;i++)  pthread_join(pw[i],NULL);
  exit(0);
}

