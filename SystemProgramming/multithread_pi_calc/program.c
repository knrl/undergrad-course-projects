/**
 * @file      program.c
 * @author    Mehmet Kaan Erol (kaanerol@marun.edu.tr)
 * @brief     System Programming 
 * @version   0.1
 * @date      2022-04-28
 * 
 * @copyright Copyright (c) 2022
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define ITER_SIZE 1000


double *th_result_ptr;

double one_iter(int start_id)
{
    int sign,start_pos,end_pos;
    double sum = 0;
    start_pos  = start_id * ITER_SIZE;
    end_pos    = (start_id+1) * ITER_SIZE;

    if (start_id % 2) sign = 1;
    else sign = -1;

    for (int i=start_pos;i<end_pos;i++) 
    {
        sign *= -1;
        sum += (double)sign / (2 * i + 1);
    }

    return sum;
}

void* thread_function(void* args)
{
    int thread_id = *(int*) args;
    th_result_ptr[thread_id] = one_iter(thread_id);
    free(args);
    return (void*)0;
}

int main(int argc, char *argv[])
{
    pthread_t *th_ptr;
    int num_of_th=0;
    int *temp_ptr;

    printf("Thread sayısını giriniz: ");
    scanf("%d", &num_of_th);

    if (num_of_th <= 0)
    {
        printf("\nThread sayısı 0'dan büyük olmalıdır.");
        return 0;
    }

    // thread'leri oluştur ve başlat
    th_ptr = calloc(num_of_th, sizeof(pthread_t));
    th_result_ptr = calloc(num_of_th, sizeof(double));
    for (int i=0;i<num_of_th;i++)
    {
        temp_ptr = (int*)malloc(sizeof(int));
        *temp_ptr= i;
        if (pthread_create(&th_ptr[i],NULL,thread_function,(void*)temp_ptr) != 0)
        {
            printf("\nThread oluşturulamadı.");
            free(th_ptr);
            free(th_result_ptr);
            return 1;
        }
    }

    // thread'lerin bitmesini bekle
    for (int i=0;i<num_of_th;i++)
    {
        pthread_join(th_ptr[i],NULL);
    }

    // sonuçları hesapla
    double pi=0.0;
    for (int i=0;i<num_of_th;i++)
    {
        pi += th_result_ptr[i];
    }
    pi *= 4;

    printf("\nToplam iterasyon: %d, Pi: %.10f\n",(num_of_th+1)*ITER_SIZE,pi);

    free(th_ptr);
    free(th_result_ptr);
    return 0;
}
