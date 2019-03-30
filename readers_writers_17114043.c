#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/errno.h>
#define THREADS_NUMB 6 
//specify the no of threads




sem_t wrt;
// semaphore to control mutual excision b/w two writers and (a writer)&(a reader) 
sem_t mutex; 
// semaphore to control mutual excision b/w two writers and two readers
int read_count=0;
void give_content()
{ int value;
  value=10;
FILE *fp;
fp= fopen ("intermediate.txt", "w");     
for(int i = 0; i < value;i++)
{
  fprintf (fp, "This is line %d\n",i + 1);}fclose(fp);}
void read_content()
{  FILE *fptr;
   fptr = fopen("intermediate.txt", "r");
   if (fptr == NULL)
      { printf("Cannot open file \n");
        exit(0);
      }
      char chh;
   chh = fgetc(fptr);
     while (chh != EOF)
      {
       printf ("%c", chh);
      chh = fgetc(fptr);}
     fclose(fptr);
}
void *writer(void *thread_n)  
{
  int n1=*(int *)thread_n;
   sem_wait(&wrt);
   give_content();
   printf("writer %d completed it's work in file\n", n1);  
   sem_post(&wrt);    
}

void *reader(void *thread_n)
{
   int n1=*(int *)thread_n;

    sem_wait(&mutex);
    read_count++;
    if(read_count==1)
    sem_wait(&wrt);
    sem_post(&mutex);
    read_content();
    printf("reader %d completed it's work in file\n", n1);
    sem_wait(&mutex);
    read_count--;
    if(read_count==0)
    sem_post(&wrt);
    sem_post(&mutex);  
}

int main(int argc, int **argv) 
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
