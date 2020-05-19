#include <stdio.h>
#include <unistd.h>
#include "../src/thread.h"

#define THREAD_NB 3
#define LOOP_NB 10
/* Nombre de millisecondes pendant lesquelles un thread
   va dormir à chaque tour de boucle */
#define MS_NB 30

void print_sleep_time(int cpt) {
    printf("THREAD %d -- %d ms se sont écoulées (%d ms / %d ms de sleep)\n",
    thread_self()->id, MS_NB, cpt, (MS_NB*LOOP_NB));
}

void *thfunc(void *ignored)
{
    int cpt = 0;
    int j;
    printf("THREAD %d -- Je rentre dans le code du thread\n", thread_self()->id);
    for(j = 0; j < LOOP_NB; j++){
        usleep(MS_NB * 1000);
        cpt += MS_NB;
        print_sleep_time(cpt);
    }
    printf("THREAD %d -- La boucle du thread est finie.\n", thread_self()->id);
    return NULL;
}

int main(int argc, char* argv[])
{
    printf("----------------------------\n31-preemption.c\n");
    thread_t threads[THREAD_NB];
    int i;

    for(i = 0; i < THREAD_NB; i++){
        thread_create(&(threads[i]),&thfunc, "thread");
    }

    int cpt = 0;
    int j;
    printf("THREAD %d -- Je rentre dans le code du parent\n", thread_self()->id);
    for(j = 0; j < LOOP_NB; j++){
        usleep(MS_NB * 1000);
        cpt += MS_NB;
        print_sleep_time(cpt);
    }
    printf("THREAD %d -- La boucle du main est finie.\n", thread_self()->id);

    return EXIT_SUCCESS;
}
