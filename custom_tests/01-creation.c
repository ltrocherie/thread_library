#include <stdio.h>
#include "../src/thread.h"

void * routine(void * arg){
  printf("AHA !\n");
  return NULL;
}

int main(int argc, char* argv[]){
  printf("----------------------------\n01-creation.c\n");
  thread_t th, th2;
  void * arg = NULL;
  thread_create(&th, routine, arg);
  thread_create(&th2, routine, arg);
  return 0;
}
