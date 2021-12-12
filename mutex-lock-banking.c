#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#define INIT_BALANCE 50
#define NUM_TRANS 100

int balance = INIT_BALANCE;

int credits = 0;
int debits = 0;
pthread_mutex_t mutex;

void *transactions(void *args)
{
    int i, v;

    for (int i = 0; i < NUM_TRANS; i++)
    {
        srand(time(NULL));
        v = rand() % NUM_TRANS;
        pthread_mutex_lock(&mutex);
        if (rand() % 2)
        {
            balance += v;
            credits += v;
        }
        else
        {
            balance -= v;
            debits += v;
        }
        pthread_mutex_unlock(&mutex);
    }

    return 0;
}
int main(int argc, char *argv[])
{
    clock_t start, end;
    start = clock();
    pthread_mutex_init(&mutex, NULL);
    int n_threads, i;
    pthread_t *threads;
    //error check
    if (argc < 2)
    {
        fprintf(stderr, "ERROR: Require number of threads\n");
        exit(1);
    }
    //convert string to int
    n_threads = atol(argv[1]);
    //error check
    if (n_threads <= 0)
    {
        fprintf(stderr, "ERROR: Invalivd value for number of threads\n");
        exit(1);
    }
    //allocate array of thread identifiers
    threads = calloc(n_threads, sizeof(pthread_t));
    //start all threads
    for (i = 0; i < n_threads; i++)
    {
        pthread_create(&threads[i], NULL, transactions, NULL);
    }
    //wait for all threads finish its jobs
    for (i = 0; i < n_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }
    printf("\tCredits:\t%d\n", credits);
    printf("\t Debits:\t%d\n\n", debits);
    printf("%d+%d-%d = \t%d\n", INIT_BALANCE, credits, debits,
           INIT_BALANCE + credits - debits);
    printf("\t Balance:\t%d\n", balance);
    //free array
    free(threads);
    pthread_mutex_destroy(&mutex);
    end = clock();

    double time_use = (double)(end - start) / CLOCKS_PER_SEC; //Tính thời gian sử dụng
    printf("Time: %lf\n", time_use);
    return 0;
}