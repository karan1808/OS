#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/errno.h>

#define CUSTOMER_NUMB 20
//give the no of CUSTOMER_NUMB 
#define NUMB_FREESEATS 6  
// maximum no of NUMB_FREESEATS

sem_t Customers; 
// makes barber sleeps if no customers   
sem_t Barber;
// used to call customers to cutting chair
sem_t Seats;
//  to protect Free_Seats variable
    
int Free_Seats=NUMB_FREESEATS;
void *barber(void *thread_n)
{
   int n1=*(int *)thread_n;
   while(1) {  /* waits for a customer (sleeps). */
            sem_wait(&Customers);
            /* mutex to protect the number of available seats.*/
            sem_wait(&Seats); 
            /* a chair gets free.*/
            Free_Seats++;
            sleep(10);
            /* bring customer for haircut.*/
            sem_post(&Barber); 
             /* release the mutex on the chair.*/
            sem_post(&Seats); 
            /* barber is cutting hair.*/
      }
}
void *customer(void *thread_n)
{       int n1=*(int *)thread_n;
        printf("%dstarted\n", n1);
         sem_wait(&Seats); 
          if(Free_Seats > 0) 
          {
                  /* sitting sem_wait.*/
                Free_Seats--; 
                
                   /* notify the barber. */
                 sem_post(&Customers); 
                       /* release the lock */
                 sem_post(&Seats); 
                    /* wait in the waiting room if barber is busy. */
                 
                 sem_wait(&Barber);
                 
                 
                  // customer is having hair cut 
            }else 
            {   /* release the lock */
               printf("%dwentout\n", n1);     
                  sem_post(&Seats); 
                  // customer leaves 
            }     
}
int main(int argc, int **argv) 
{
   sem_init(&Customers,0,0);
   sem_init(&Barber,0,0);
   sem_init(&Seats,0,1);
   pthread_t thread_customers[CUSTOMER_NUMB];
   pthread_t thread_barber[1];
   int thread_numb[CUSTOMER_NUMB];
   int x=1;
   pthread_create(thread_barber+0, NULL, barber,&x); 
   for (int i = 0; i < CUSTOMER_NUMB;i++ ) 
   {
      thread_numb[i] = i;
      pthread_create(thread_customers + i, NULL, customer,thread_numb + i);        
   }
   for (int i = 0; i < CUSTOMER_NUMB; i++)
   {
      pthread_join(thread_customers[i], NULL);
   }
}





