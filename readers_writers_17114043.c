#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/errno.h>
#define THREADS_NUMB 6 
//specify the no of threads




sem_t wrt;
sem_t mutex; 


int read_count=0;

void data_read()
{  
	FILE *file_reader;
     file_reader = fopen("intermediate.txt", "r");
   
      char chh;
      chh = fgetc(file_reader);
      
       while (chh != EOF)
       {
           printf ("%c", chh);
           chh = fgetc(file_reader);
       }
     
     fclose(file_reader);
}



void data_specify()
{ 

     FILE *file_reader;
     file_reader= fopen ("intermediate.txt", "w");     
     
     for(int i = 0; i < rand()%50;i++)
     {
       fprintf (file_reader, "This is line %d\n",i + 1);
     }

     
     fclose(file_reader);

}






void *reader(void *thread_n)
{
   int numb=*(int *)thread_n;

    sem_wait(&mutex);
        read_count++;
        if(read_count==1)
    sem_wait(&wrt);
    
    sem_post(&mutex);
    
    data_read();
    
    
    sem_wait(&mutex);
         read_count--;
         if(read_count==0)
    sem_post(&wrt);
    
    sem_post(&mutex);  
}


void *writer(void *thread_n)  
{
   int numb=*(int *)thread_n;
   
   sem_wait(&wrt);
      data_specify();
      printf("writer %d completed it's work in file\n", numb);  
   sem_post(&wrt);    

}









int main() 
{
   sem_init(&wrt,0,1); 
   sem_init(&mutex,0,1);  
   pthread_t thread_read[THREADS_NUMB];
   pthread_t thread_write[THREADS_NUMB];
   int thread_numb[THREADS_NUMB];
   for (int i = 0; i < THREADS_NUMB;i++ ) 
   {
       thread_numb[i] = i;
       pthread_create(thread_read + i,NULL, reader,thread_numb + i);
       pthread_create(thread_write + i,NULL, writer,thread_numb + i);      
   }

   for (int i = 0; i < THREADS_NUMB; i++)
   {
      pthread_join(thread_write[i], NULL);
      pthread_join(thread_read[i], NULL); 
   }
}
