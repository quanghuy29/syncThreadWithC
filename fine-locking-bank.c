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
pthread_mutex_t b_lock, c_lock, d_lock;

void *transactions(void *args)
{
    int i, v;

    for (int i = 0; i < NUM_TRANS; i++)
    {
        srand(time(NULL));
        v = rand() % NUM_TRANS;
        if (rand() % 2)
        {
            pthread_mutex_lock(&b_lock);
            balance += v;
            pthread_mutex_unlock(&b_lock);

            pthread_mutex_lock(&c_lock);
            credits += v;
            pthread_mutex_unlock(&c_lock);
        }
        else
        {
            pthread_mutex_lock(&b_lock);
            balance -= v;
            pthread_mutex_unlock(&b_lock);

            pthread_mutex_lock(&d_lock);
            debits += v;
            pthread_mutex_unlock(&d_lock);
        }
    }

    return 0;
}
int main(int argc, char *argv[])
{
    clock_t start, end;
    start = clock();
    pthread_mutex_init(&b_lock, NULL);
    pthread_mutex_init(&c_lock, NULL);
    pthread_mutex_init(&d_lock, NULL);
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
    pthread_mutex_destroy(&b_lock);
    pthread_mutex_destroy(&c_lock);
    pthread_mutex_destroy(&d_lock);

    free(threads);
    end = clock();

    double time_use = (double)(end - start) / CLOCKS_PER_SEC; //T??nh th???i gian s??? d???ng
    printf("Time: %lf\n", time_use);

    return 0;
}