/*------------------------------------------------------------------------------
*
* @author: Mehmet Kaan Erol
*
------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

#define NUM_BARBER 2

// global variable declaration
sem_t semBarber, semCustomer;
pthread_t *thCustomerArray;
pthread_t thBarberArray[NUM_BARBER];
pthread_mutex_t mutexChair;
int count = 0, maxCount;

void* customer();
void* barber();
int main()
{
    pthread_mutex_init(&mutexChair, NULL);
    unsigned int numCustomer = 0, waitChair = 0;
    int i;
    printf("Musteri sayisi: ");
    scanf("%d",&numCustomer);

    printf("Bekleme koltugu sayisi: ");
    scanf("%d",&waitChair);

    // max chair value
    maxCount = waitChair;
    // initial barber number is 2
    sem_init(&semBarber, 0, NUM_BARBER);
    // initial customer number is 0
    sem_init(&semCustomer, 0, 0);

    // allocate space for customer threads than create threads 
    thCustomerArray = (pthread_t*)malloc(sizeof(pthread_t)*numCustomer);
    for (i=0;i<numCustomer;i++)
    {
        // if pthread_create return 0 is mean error
        // Returns the possix thread id if different from 0
        // but this id is not the real id that used by os  
        if (pthread_create(thCustomerArray+i, NULL, customer, NULL) != 0)
        {
            perror("Give some error");
        }
    }

    // create barber threads
    pthread_create(thBarberArray, NULL, barber, NULL);      // barber thread 1
    pthread_create(thBarberArray + 1, NULL, barber, NULL);  // barber thread 2

    // wait for threads to terminate than returns the threads exit status
    // but for this example there is no return value
    pthread_join(thBarberArray[0], NULL);  
    pthread_join(thBarberArray[1], NULL);

    for (i=0;i<numCustomer;i++)
    {
        // check join thread whether there is detached thread or not
        if (pthread_join(thCustomerArray[i], NULL) != 0)
        {
            perror("Give some error");
        }
    }

    sem_destroy(&semBarber);
    sem_destroy(&semCustomer);
    pthread_mutex_destroy(&mutexChair);
    return 0;
}

void* barber()
{
    while (count != 0)
    {
        sem_wait(&semCustomer);
        pthread_mutex_lock(&mutexChair);
        // critical section
        count--;
        pthread_mutex_unlock(&mutexChair);
        printf("%d. musteri tiras oldu.\n", count+1);
        sem_post(&semBarber);
        sleep(1);
    }
}

void* customer()
{
    pthread_mutex_lock(&mutexChair);
    if (count<maxCount)
    {
        count++;
        printf("Bekleyen musteri sayisi %d\n", count);
        pthread_mutex_unlock(&mutexChair);
        sem_post(&semCustomer);
        sem_wait(&semBarber);
        printf("Musteri tras oldu.\n");
    }
    else
    {
        printf("Musteriyi kaybettik.\n");
        pthread_mutex_unlock(&mutexChair);
    }
    sleep(3);
}
