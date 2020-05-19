#include <stdio.h>
#include <ucontext.h>
#include "../src/thread.h"

void * g(void * args)
{
  printf("Thread number %p started\n", thread_self());
  thread_exit(args);
  return NULL;
}

void * f(void * arg)
{
  printf("Thread number %p started\n", thread_self());

  thread_t th2;
  int err = thread_create(&th2, g, NULL);
  printf("The return code is %d with the thread %p\n",err, th2);

  thread_exit(arg);
  printf("Should not be displayed\n" );
  return NULL;
}

int main(int argc, char const *argv[]) {
  printf("----------------------------\n04-several_threads.c\n");
  thread_t th1;
  printf("Main started\n");
  int err = thread_create(&th1, f, NULL);
  printf("The return code is %d with the thread %p\n",err, th1);
  printf("Main ended \n");
  void *retval = NULL;
  thread_exit(retval);
  return 0;
}
