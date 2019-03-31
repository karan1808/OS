#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/errno.h>


 
#define NUMB_THREADS 6
#define SIZE_D 5

 
typedef int BUFFER_ARRAY;
BUFFER_ARRAY buffer[SIZE_D];  

int last_pointer;


sem_t mutex_BUUFER_ARRAY;
sem_t COMPLETE_FULL;
sem_t EMPTY;
 
 



void insert_in_array(BUFFER_ARRAY value)  
{
    if (index_buffer < SIZE_D) 
    {
       buffer[index_buffer++] = value;
    } 
    
}
 
BUFFER_ARRAY consuming_array() 
{
    if (index_buffer > 0) { return buffer[--index_buffer];} 
}
 

void *consumer(void *thread_n) 
{
    int numb_thread = *(int *)thread_n;
    printf("entered consumer %d\n",numb_thread);
    BUFFER_ARRAY value;
    int i=0;
      
        sem_wait(&EMPTY);
        sem_wait(&mutex_BUUFER_ARRAY);
        value = consuming_array(value);
        sem_post(&mutex_BUUFER_ARRAY);
        sem_post(&COMPLETE_FULL); 
        printf("Consumer %d dequeue %d from buffer\n", numb_thread, value);
   
    
}



 
void *producer(void *thread_n) 
{
    int numb_thread = *(int *)thread_n;
    printf("entered producer %d\n",numb_thread);
    BUFFER_ARRAY value;
    int i=0;
        sleep(rand() % 10);
        value = rand() % 100;
        sem_wait(&COMPLETE_FULL); 
        sem_wait(&mutex_BUUFER_ARRAY);
        insert_in_array(value);
        sem_post(&mutex_BUUFER_ARRAY);
        sem_post(&EMPTY); 
        printf("Producer %d added %d to buffer\n", numb_thread, value);
        
}








 
int main() 
{
    
    index_buffer = 0;
 
    sem_init(&mutex_BUUFER_ARRAY,0,1);
    sem_init(&COMPLETE_FULL,0,SIZE_D);
    sem_init(&EMPTY,0,0);
    

    pthread_t thread_producer[NUMB_THREADS];  
    pthread_t thread_consumer[NUMB_THREADS];
    


    int numb_thread[NUMB_THREADS];
    for (int i = 0; i < NUMB_THREADS;i++) 
    {

     numb_thread[i] = i;
     pthread_create(thread_producer + i,NULL,producer,numb_thread + i);
     pthread_create(thread_consumer + i,NULL,consumer,numb_thread + i);
    
    }
    
    for (int i = 0; i < NUMB_THREADS; i++)
    { 
    	pthread_join(thread_producer[i], NULL);
        pthread_join(thread_consumer[i], NULL);
    }
    
    return 0;
}
