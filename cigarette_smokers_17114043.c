#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/errno.h>

#define NUMBER_OF_SMOKERS 3


sem_t provider;
sem_t Tobacco; 
sem_t Paper;
sem_t Match;

_Bool isTobacco=0,isPaper=0,isMatch=0;


sem_t invoking_tabbbco_person;

sem_t invoking_paper_pesron ;

sem_t invoking_paper_person;  



sem_t mutex;


_Bool satisfied[NUMBER_OF_SMOKERS];
  


void producing()
{
  sleep(15);
}

void *agent()
{  
 while(1)
{
      sem_wait(&provider);  
      sem_wait(&mutex);
      int choose=rand()%3;
      while(satisfied[choose])
      {  
           choose=(choose+1)%((int)NUMBER_OF_SMOKERS);
        } 
    if(choose!=0)
    {
    sem_post(&Tobacco);
    }
    if(choose!=1)
    {
    sem_post(&Paper);  
    }
    if(choose!=2)
    {
    sem_post(&Match);
      }
    sem_post(&mutex);
    }       
}



void *match()
{
  sem_wait(&invoking_paper_person);
  printf("person_with_mathes got chance\n");
  producing();
    satisfied[2]=1;
    printf("person_with_mathes satisfied with this\n");
  sem_post(&provider);   
}

void *tobacco()
{

  sem_wait(&invoking_tabbbco_person);
  printf("person_with_tabbco got chance\n");
  producing();
  satisfied[0]=1;
  printf("person_with_tabbabco satisfied with this\n");
  sem_post(&provider);   
}

void *paper()
{
  sem_wait(&invoking_paper_pesron );
  printf("person_with_paper got chance\n");
  producing();
  satisfied[1]=1;
  printf("person_with_paper satisfied with this\n");
  sem_post(&provider);   
}




void *pusherA()
{while(1)
  {
  sem_wait(&Tobacco);
  sem_wait(&mutex);
    if(isPaper)
    {   isPaper=0;
      printf("giving the ingredients to the person with matches");
        sem_post(&invoking_paper_person);
      }
    else if(isMatch)
    { isMatch=0;
       printf("giving the ingredients to the person with paper");
         sem_post(&invoking_paper_pesron );
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
      printf("giving the ingredients to the person with tobacco");
      sem_post(&invoking_tabbbco_person); 
    }
    else if(isTobacco)
    {
       isTobacco=0;
      printf("giving the ingredients to the person with matches");
       sem_post(&invoking_paper_person);
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
      printf("giving the ingredients to the person with tobacco");
      sem_post(&invoking_tabbbco_person); 
    }
    else if(isTobacco)
    {
       isTobacco=0;
       printf("giving the ingredients to the person with paper");
       sem_post(&invoking_paper_pesron );
    }
    else
    {
        isMatch=1;
    }
     
     sem_post(&mutex);
        
    }     
}



int main() 
{
    
    
     sem_init(&provider,0,1);
     sem_init(&invoking_paper_pesron ,0,0);
     sem_init(&invoking_paper_person,0,0);
     sem_init(&invoking_tabbbco_person,0,0);
     sem_init(&Paper,0,0);
     sem_init(&Match,0,0);
     sem_init(&Tobacco,0,0);

     sem_init(&mutex,0,1);


    for (int i = 0; i < NUMBER_OF_SMOKERS; ++i)
    {
      satisfied[i]=0;
    }


     pthread_t thread[NUMBER_OF_SMOKERS];
     pthread_t pushers[NUMBER_OF_SMOKERS];
     
     pthread_create(thread + 0,NULL,*tobacco,NULL);
     pthread_create(thread + 1,NULL,*match,NULL);
     pthread_create(thread + 2,NULL,*paper,NULL);  
     
     pthread_create(pushers + 0,NULL,*pusherA,NULL);
     pthread_create(pushers + 1,NULL,*pusherB,NULL);
     pthread_create(pushers + 2,NULL,*pusherC,NULL);
     
     pthread_t Agent[1];
     pthread_create(Agent+0,NULL,*agent,NULL);   
    
     int i=0;
    while(i<NUMBER_OF_SMOKERS)
    {
       pthread_join(thread[i],NULL);
      i++;
    }
  
  
        
    
    return 0;
} 
