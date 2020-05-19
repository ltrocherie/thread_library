#include <stdio.h>
#include <ucontext.h>
#include "../src/thread.h"

void * f(void * arg)
{
  printf("Thread number %p started\n", thread_self());
  thread_exit(arg);
  printf("Should not be displayed\n" );
  return NULL;
}

int main(int argc, char const *argv[]) {
  printf("----------------------------\n03-creation_and_destruction.c\n");
  thread_t th;
  printf("Main started\n");
  int err = thread_create(&th, f, NULL);
  printf("The return code is %d with the thread %p\n",err, th);
  printf("Main ended \n");
  void *retval = NULL;
  thread_exit(retval);
  return 0;
}
