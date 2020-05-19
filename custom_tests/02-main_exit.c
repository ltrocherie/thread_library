#include <stdio.h>
#include <ucontext.h>
#include "../src/thread.h"


int main(int argc, char const *argv[]) {
  printf("----------------------------\n02-main_exit.c\n");
  printf("\n");
  void * ret = NULL;
  printf("Thread number %p (main)\n", thread_self());
  thread_exit(ret);
  printf("Should not be displayed\n");
  return 0;
}
