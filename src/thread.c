#include "thread.h"
#include <sys/time.h>
#include <signal.h>

#define SUCCESS 0
#define FAILURE -1


// For the main execution
extern int main(int, char**);
static int argc_th;
static char **argv_th;

os_context os_ctxt;

/*
 * Preemption Variables (for timer)
 */
 struct itimerval timer;
 int timer_sec = 0;
 int timer_usec = 100000; // 100 ms is the default value for a Linux timeslice

////////////////////////////////////////////////////////////////////////////////
/* Auxilary functions */

/*
 * Return the value of the first available id
 */
int get_available_id(){
  static int id_count = 0;
  id_count++;
  return id_count;
}

/*
 * The thread stand aside if it exceeds the execution time which is authorized to him
 */
void alarm_handler(int signum) {
    int thread_id = 0;
    if(thread_self() != NULL) // It can happens that self == null, if the function launches after the thread has been finished for example
        thread_id = thread_self()->id;

    if(signum == SIGALRM) {
        printf("[DEBUG] Je suis le thread %d et j'ai pris trop de temps à m'exécuter ! \n", thread_id);
        thread_yield();
    }
}

/*
 * Sets the signal handler for SIGALRM
 */
void setting_handler() {
  struct sigaction sa;
  sa.sa_handler = alarm_handler; // Function called when signal is received
  sa.sa_flags = 0; // No options
  sigemptyset(&sa.sa_mask); // Set of signal blocked during the execution of the handler (here none)
  sigaction(SIGALRM, &sa, NULL); // Sets the new handler for SIGALRM
}

// executes a new thread
void thread_exec(void *(*func)(void *, void*), void *funcarg) {
    void *retval;

    //setting timer for preemption
    timer.it_value.tv_sec = timer_sec;
    timer.it_value.tv_usec = timer_usec;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    setting_handler();
    setitimer(ITIMER_REAL, &timer, NULL);

    // executes the main at first
    if(func == (void*(*)(void*, void*))main) {
        main(argc_th, argv_th);
        retval = NULL;
    }
    else
        retval = (*func)(funcarg, NULL);

    // Does a thread_exit if the thread didn't do one
    thread_exit(retval);
}

// instanciates a new thread and pushes it on the queue
thread_t thread_push(int thread_id, void *(*func)(void *), void *funcarg){

  // gets the current context
  ucontext_t *current_ctxt = malloc(sizeof(ucontext_t));
  int err = getcontext(current_ctxt);
  if(err)
    return NULL;

  // fills in the new context arguments
  ucontext_t *new_ctxt = current_ctxt;
  new_ctxt->uc_link = NULL;
  new_ctxt->uc_stack.ss_size = THREAD_STACK_SIZE;
  new_ctxt->uc_stack.ss_sp = malloc(new_ctxt->uc_stack.ss_size);
  int valgrind_stackid_tmp = VALGRIND_STACK_REGISTER(new_ctxt->uc_stack.ss_sp, new_ctxt->uc_stack.ss_sp + new_ctxt->uc_stack.ss_size);

  // creates the new thread
  thread_t new_thread = malloc(sizeof(thread));
  new_thread->id = thread_id;
  new_thread->context = new_ctxt;
  new_thread->is_finished = 0;
  new_thread->retval = 0;
  new_thread->valgrind_stackid = valgrind_stackid_tmp;
  STAILQ_INIT(&new_thread->thread_waiting_list);

  // creates the new context (returns void)
  makecontext(new_ctxt, (void (*)(void))thread_exec, 2, func, funcarg);

  STAILQ_INSERT_TAIL(&thread_runqueue, new_thread, next);

  return new_thread;
}

void thread_free(thread_t th){
  thread_t waiting_thread = STAILQ_FIRST(&th->thread_waiting_list);

  while (waiting_thread != NULL)
  {
    thread_t next_thread = STAILQ_NEXT(waiting_thread, next);
    STAILQ_REMOVE_HEAD(&th->thread_waiting_list, next);
    thread_free(waiting_thread);
    waiting_thread = next_thread;
  }
  VALGRIND_STACK_DEREGISTER(th->valgrind_stackid);
  free(th->context->uc_stack.ss_sp);
  free(th->context);
  free(th);
  return;
}


////////////////////////////////////////////////////////////////////////////////

/* Applies the constructor attribute to init_thread_library() so that it is executed before main() */
void init_thread_library (int argc, char **argv) __attribute__ ((constructor));

void init_thread_library (int argc, char **argv){
  // Initializes the queues
  STAILQ_INIT(&thread_runqueue);

  argc_th = argc;
  argv_th = argv;

  // Stores the main
  thread_t main_th = thread_push(0, (void *(*)(void*))main, NULL);
  int err = getcontext(&os_ctxt.context);
  if(err)
    return;
  err = swapcontext(&os_ctxt.context, main_th->context);
  if(err)
    return;

  exit(0);
}

/* Applies the destructor attribute to close_thread_library() so that it is executed after main() */
void close_thread_library (void) __attribute__ ((destructor));

void close_thread_library (void){
  thread_free(os_ctxt.last_thread);

  thread_t dead_thread = STAILQ_FIRST(&thread_deadstack);

  while (dead_thread != NULL)
  {
    thread_t next = STAILQ_NEXT(dead_thread, next);
    thread_free(dead_thread);
    dead_thread = next;
  }


  exit(0);
}

////////////////////////////////////////////////////////////////////////////////


int thread_create(thread_t *newthread, void *(*func)(void *), void *funcarg) {
  // returns if the parameters are nil
  if (newthread == NULL || func == NULL)
        return FAILURE;

  *newthread = thread_push(get_available_id(), func, funcarg);

  return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////

thread_t thread_self(void) {
  //gets the current thread
  thread_t current_thread = STAILQ_FIRST(&thread_runqueue);
  return current_thread;
}

////////////////////////////////////////////////////////////////////////////////

int thread_yield(void) {
  // gets the ongoing thread and removes it
  thread_t th_current = STAILQ_FIRST(&thread_runqueue);
  if (th_current == NULL)
    return FAILURE;
  STAILQ_REMOVE_HEAD(&thread_runqueue, next);

  // puts it at the tail of the runqueue
  STAILQ_INSERT_TAIL(&thread_runqueue, th_current, next);

  //setting timer for preemption
  struct itimerval timer;
  timer.it_value.tv_sec = timer_sec;
  timer.it_value.tv_usec = timer_usec;
  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_usec = 0;
  setting_handler();
  setitimer(ITIMER_REAL, &timer, NULL);

  // gets the next context that is ready and swaps it
  thread_t th_new = STAILQ_FIRST(&thread_runqueue);
  int err = swapcontext(th_current->context, th_new->context);
  return err;
}


////////////////////////////////////////////////////////////////////////////////


int thread_join(thread_t th, void **retval) {
  int err = SUCCESS;
  if (th == NULL)
    return FAILURE;
  // Active wait for the thread to end
  if(!th->is_finished){
    // Gets the ongoing thread
    thread_t th_current = STAILQ_FIRST(&thread_runqueue);
    if (th_current == NULL)
      return FAILURE;
    // Removes it of the runqueue and puts it in the waiting list
    STAILQ_REMOVE_HEAD(&thread_runqueue, next);
    STAILQ_INSERT_HEAD(&th->thread_waiting_list, th_current, next);

    // gets the next context that is ready and swaps it
   thread_t th_new = STAILQ_FIRST(&thread_runqueue);
   err = swapcontext(th_current->context, th_new->context);
  }

  if(retval != NULL)
    *retval = th->retval;
  return err;
}

////////////////////////////////////////////////////////////////////////////////

void thread_exit(void *retval)
{
  // gets the context of the current thread and removes it of the ready_list
  thread_t th_current = STAILQ_FIRST(&thread_runqueue);
  STAILQ_REMOVE_HEAD(&thread_runqueue, next);

  // sets the attribute of the current context to the right values
  th_current->is_finished = 1;
  th_current->retval = retval;

  // moves back all the threads waiting for this one in the runqueue
  thread_t waiting_thread = STAILQ_FIRST(&th_current->thread_waiting_list);
  while (waiting_thread != NULL)
  {
    thread_t next_thread = STAILQ_NEXT(waiting_thread, next);
    STAILQ_REMOVE_HEAD(&th_current->thread_waiting_list, next);
    STAILQ_INSERT_TAIL(&thread_runqueue, waiting_thread, next);
    waiting_thread = next_thread;
  }

  thread_t th_new = STAILQ_FIRST(&thread_runqueue);
  if(th_new == NULL) { // ends the program and goes to the destructor
        os_ctxt.last_thread = th_current;
        setcontext(&os_ctxt.context);
  }

  //pushes the old thread in the dead stack
  STAILQ_INSERT_HEAD(&thread_deadstack, th_current, next);

  setcontext(th_new->context);
  exit(1);

}

////////////////////////////////////////////////////////////////////////////////
/*
  Mutex Implementation

  */


int thread_mutex_init(thread_mutex_t *mutex)
{
  if(mutex == NULL)
    return FAILURE;
  if(!mutex->is_initialized)
  {
    mutex->thread_locker= NULL;
    mutex->is_initialized = 1;
    STAILQ_INIT(&mutex->mutex_waiting_list);
    return SUCCESS;
  }
  return FAILURE;

}
int thread_mutex_destroy(thread_mutex_t *mutex)
{
  if(mutex == NULL)
    return FAILURE;
  if(!mutex->is_initialized)
    return FAILURE;
  mutex->is_initialized = 0;
  mutex->thread_locker = NULL;
  return SUCCESS;
}

int thread_mutex_lock(thread_mutex_t *mutex)
{
  if(mutex == NULL || mutex->is_initialized == 0)
    return FAILURE;


  // Gets the ongoing thread
  thread_t th_current = STAILQ_FIRST(&thread_runqueue);
  if (th_current == NULL)
    return FAILURE;
  thread_t previous_locker = __sync_lock_test_and_set(&(mutex->thread_locker), th_current);
  if(previous_locker == NULL) // Only works on one core
    return SUCCESS;

  // Another thread is using this lock
  mutex->thread_locker = previous_locker;
  // Removes it of the runqueue and puts it in the waiting list
  STAILQ_REMOVE_HEAD(&thread_runqueue, next);
  STAILQ_INSERT_HEAD(&mutex->mutex_waiting_list, th_current, next);

  // gets the next context that is ready and swaps it
  thread_t th_new = STAILQ_FIRST(&thread_runqueue);
  swapcontext(th_current->context, th_new->context);
  return SUCCESS;
}

int thread_mutex_unlock(thread_mutex_t *mutex)
{
  if(mutex == NULL || !mutex->is_initialized || mutex->thread_locker != STAILQ_FIRST(&thread_runqueue))
  {
    printf("Error\n");
    return FAILURE;
  }

  // moves back all a thread waiting for the mutex in the runqueue
  thread_t waiting_thread = STAILQ_FIRST(&mutex->mutex_waiting_list);
  if(waiting_thread != NULL)
  {
    STAILQ_REMOVE_HEAD(&mutex->mutex_waiting_list, next);
    STAILQ_INSERT_TAIL(&thread_runqueue, waiting_thread, next);
    mutex->thread_locker = waiting_thread;
  }else{
    mutex->thread_locker = NULL;
  }
  return SUCCESS;
}
