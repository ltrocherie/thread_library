#include <stdio.h>
#include <sys/time.h>
#include <assert.h>
#include "../src/thread.h"

#define array_size 10000

int a[array_size];
int global_index = 0;
int sum = 0;
int no_threads = 10;
thread_mutex_t lock;

void* compute(void * unused){
    // Each thread computes sum of a part of the array
    thread_mutex_lock(&lock);
    int thread_part = global_index++;
    thread_mutex_unlock(&lock);

    int i, local_sum = 0;

    for (i = thread_part * (array_size / no_threads); i < (thread_part + 1) * (array_size / no_threads); i++)
        local_sum += a[i];

    thread_mutex_lock(&lock);
    sum += local_sum;
    thread_mutex_unlock(&lock);
    thread_exit(NULL);
}

int main(int argc, char* argv[])
{
    printf("----------------------------\n21-large-array-sum.c\n");
    int i, err;
    if(argc > 1)
        no_threads = atoi(argv[1]);

    thread_t *th = malloc(no_threads*sizeof(*th));

    if (thread_mutex_init(&lock) != 0) {
        fprintf(stderr, "thread_mutex_init failed\n");
        return -1;
    }

    for (i = 0; i < array_size; i++){
        a[i] = i + 1;
    }

    struct timeval tv1, tv2;

    gettimeofday(&tv1, NULL);
    /* on cree tous les threads */
    for(i=0; i<no_threads; i++) {
        err = thread_create(&th[i], compute, NULL);
        assert(!err);
    }

    /* on leur passe la main, ils vont tous terminer */
    for(i=0; i<no_threads; i++) {
        thread_yield();
    }

    /* on les joine tous, maintenant qu'ils sont tous morts */
    for(i=0; i<no_threads; i++) {
        err = thread_join(th[i], NULL);
        assert(!err);
    }
    gettimeofday(&tv2, NULL);
    float s = (tv2.tv_sec-tv1.tv_sec) + (tv2.tv_usec-tv1.tv_usec) * 1e-6;
    printf("The sum of 1 to %d is %d in %f s.\n", array_size, sum, s);
    thread_mutex_destroy(&lock);
    free(th);
    return EXIT_SUCCESS;
}
