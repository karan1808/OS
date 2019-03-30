#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/errno.h>


 
#define NUMB_THREADS 6
#define SIZE_D 5

 
typedef int buffer_t;
buffer_t buffer[SIZE_D];  
int index_buffer;


sem_t buffer_mutex;
sem_t full_sem;
sem_t empty_sem;
 
 



void insertbuffer(buffer_t value)  
{
    if (index_buffer < SIZE_D) 
    {
       buffer[index_buffer++] = value;
    } 
    else 
    {
        printf("Buffer overflow\n");
    }
}
 
buffer_t dequeuebuffer() 
{
    if (index_buffer > 0) 
    {      return buffer[--index_buffer]; } else {printf("Buffer underflow\n");}
    return 0;
}
 
 
void *producer(void *thread_n) 
{
    int numb_thread = *(int *)thread_n;
    printf("entered producer %d\n",numb_thread);
    buffer_t value;
    int i=0;
        sleep(rand() % 10);
        value = rand() % 100;
        sem_wait(&full_sem); 
        sem_wait(&buffer_mutex);
        insertbuffer(value);
        sem_post(&buffer_mutex);
        sem_post(&empty_sem); 
        printf("Producer %d added %d to buffer\n", numb_thread, value);
        
}
 
void *consumer(void *thread_n) 
{
    int numb_thread = *(int *)thread_n;
    printf("entered consumer %d\n",numb_thread);
    buffer_t value;
    int i=0;
      
        sem_wait(&empty_sem);
        sem_wait(&buffer_mutex);
        value = dequeuebuffer(value);
        sem_post(&buffer_mutex);
        sem_post(&full_sem); 
        printf("Consumer %d dequeue %d from buffer\n", numb_thread, value);
   
    
}
 
int main(int argc, int **argv) 
{
    
    index_buffer = 0;
 
    sem_init(&buffer_mutex,
             0,
    // int pshared. 0 = shared between threads of process,  1 = shared between processes
             1);
    // value of the semaphore
    sem_init(&full_sem,0,SIZE_D);
    sem_init(&empty_sem,0,0);
    pthread_t thread_producer[NUMB_THREADS];// threads for producer   
    pthread_t thread_consumer[NUMB_THREADS];// threads for producer
    int numb_thread[NUMB_THREADS];
    for (int i = 0; i < NUMB_THREADS;i++) 
    {numb_thread[i] = i;
     pthread_create(thread_producer + i,NULL,producer,numb_thread + i);
     pthread_create(thread_consumer + i,NULL,consumer,numb_thread + i);
    }
     for (int i = 0; i < NUMB_THREADS; i++)
    { pthread_join(thread_producer[i], NULL);
      pthread_join(thread_consumer[i], NULL);
    }
    return 0;
}
