#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/errno.h>

#define NUMBER_OF_SMOKERS 3

//personA tobacco

//personB paper

//personC match


sem_t agentsem;
sem_t Tobacco; 
sem_t Paper;
sem_t Match;

_Bool isTobacco=0,isPaper=0,isMatch=0;


sem_t Tobaccosem;

sem_t Papersem;

sem_t Matchsem;  



sem_t mutex;


_Bool done[NUMBER_OF_SMOKERS];  


void make()
{
  int val=rand()%3;
  printf("val %d\n",val);
  sleep(val);
}
void *agent()
{   while(1)
    {
      sem_wait(&agentsem);  
      sem_wait(&mutex);
      int select=rand()%3;
        while(done[select])
        {  int k1=NUMBER_OF_SMOKERS;
           select=(select+1)%k1;
        } 
    if(select!=0)
    {
    sem_post(&Tobacco);
    }
    if(select!=1)
    {
    sem_post(&Paper);  
    }
    if(select!=2)
    {
    sem_post(&Match);
      }
    sem_post(&mutex);
    }       
}
void *pusherA()
{while(1)
  {
  sem_wait(&Tobacco);
  sem_wait(&mutex);
    if(isPaper)
    {   isPaper=0;
      printf("invoked personC");
        sem_post(&Matchsem);
      }
    else if(isMatch)
    { isMatch=0;
       printf("invoked personB");
         sem_post(&Papersem);
    }
    else
    {
        isTobacco=1;
    }
    sem_post(&mutex);
   }
}

void *pusherB()
{
    while(1)
  {     sem_wait(&Paper);
   sem_wait(&mutex);
    if(isMatch)
    {
      isMatch=1; 
      printf("invoked personA");
      sem_post(&Tobaccosem); 
    }
    else if(isTobacco)
    {
       isTobacco=0;
       printf("invoked personC");
       sem_post(&Matchsem);
    }
    else
    {
        isPaper=1;
    }
    sem_post(&mutex);
    }
}
void  *pusherC()
{
     while(1)
  {
   sem_wait(&Match);
   sem_wait(&mutex);
    if(isPaper)
    {
      isPaper=1; 
      printf("invoked personA");
      sem_post(&Tobaccosem); 
    }
    else if(isTobacco)
    {
       isTobacco=0;
       printf("invoked personB");
       sem_post(&Papersem);
    }
    else
    {
        isMatch=1;
    }
     
     sem_post(&mutex);
        
    }     
}


void *tobacco()
{

  sem_wait(&Tobaccosem);
  printf("personA got chance\n");
  make();
  done[0]=1;
  printf("personA done with this\n");
  sem_post(&agentsem);   
}
void *paper()
{
  sem_wait(&Papersem);
  printf("personB got chance\n");
  make();
  done[1]=1;
  printf("personB done with this\n");
  sem_post(&agentsem);   
}

void *match()
{
  sem_wait(&Matchsem);
  printf("personC got chance\n");
  make();
    done[2]=1;
    printf("personC done with this\n");
  sem_post(&agentsem);   
}


int main(int argc, int **argv) 
{
    
    for (int i = 0; i < NUMBER_OF_SMOKERS; ++i)
    {
      done[i]=0;
    }
     sem_init(&agentsem,0,1);
     sem_init(&Papersem,0,0);
     sem_init(&Matchsem,0,0);
     sem_init(&Tobaccosem,0,0);
     sem_init(&Paper,0,0);
     sem_init(&Match,0,0);
     sem_init(&Tobacco,0,0);

     sem_init(&mutex,0,1);


     pthread_t thread[NUMBER_OF_SMOKERS];
     pthread_t pushers[NUMBER_OF_SMOKERS];
     
     pthread_create(thread + 0,NULL,*tobacco,NULL);
     pthread_create(pushers + 0,NULL,*pusherA,NULL);
     pthread_create(thread + 1,NULL,*match,NULL);
     pthread_create(pushers + 1,NULL,*pusherB,NULL);
     pthread_create(thread + 2,NULL,*paper,NULL);  
     pthread_create(pushers + 2,NULL,*pusherC,NULL);
     pthread_t Agent[1];
     pthread_create(Agent+0,NULL,*agent,NULL);   
    for (int i = 0; i < NUMBER_OF_SMOKERS; ++i)
      {
         pthread_join(thread[i],NULL);
      }    
    return 0;
} 
