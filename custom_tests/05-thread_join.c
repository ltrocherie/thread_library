#include <stdio.h>
#include <ucontext.h>
#include "../src/thread.h"

thread_t th1, th2;

void * f(void * arg)
{
  printf("Thread number %p started\n", thread_self());
  thread_join(th2, NULL);
  thread_exit(NULL);
  printf("Should not be displayed\n" );
  return NULL;
}

void * g(void * arg)
{
  printf("Thread number %p started\n", thread_self());
  thread_exit(NULL);
  printf("Should not be displayed\n" );
  return NULL;
}

int main(int argc, char const *argv[]) {
  printf("----------------------------\n05-thread_join.c\n");
  printf("Main started\n");
  thread_create(&th1, f, NULL);
  thread_create(&th2, g, NULL);
  thread_join(th2, NULL);
  printf("Main ended \n");
  return 0;
}
