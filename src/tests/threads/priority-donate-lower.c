/* The main thread acquires a lock.  Then it creates a
   higher-priority thread that blocks acquiring the lock, causing
   it to donate their priorities to the main thread.  The main
   thread attempts to lower its priority, which should not take
   effect until the donation is released. */

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/synch.h"
#include "threads/thread.h"

static thread_func acquire_thread_func;

void
test_priority_donate_lower (void) 
{
  struct lock lock;

  /* This test does not work with the MLFQS. */
  ASSERT (!thread_mlfqs);

  /* Make sure our priority is the default. */
  ASSERT (thread_get_priority () == PRI_DEFAULT);

  lock_init (&lock);
  lock_acquire (&lock);

  thread_create ("acquire", PRI_DEFAULT + 10, acquire_thread_func, &lock);
  printf ("(priority-donate-lower) Main thread should have priority %d.  Actual priority: %d.\n",
       PRI_DEFAULT + 10, thread_get_priority ());

  
  printf ("(priority-donate-lower) Lowering base priority...\n");

     printf("%d\n", thread_current()->base_priorities_count);


  thread_set_priority (PRI_DEFAULT - 10);
  printf ("(priority-donate-lower) Main thread should have priority %d.  Actual priority: %d.\n",
       PRI_DEFAULT + 10, thread_get_priority ());
  
  lock_release (&lock);
  printf ("(priority-donate-lower) acquire must already have finished.\n");
  printf ("(priority-donate-lower) Main thread should have priority %d.  Actual priority: %d.\n",
       PRI_DEFAULT - 10, thread_get_priority ());
}

static void
acquire_thread_func (void *lock_) 
{
  struct lock *lock = lock_;

  lock_acquire (lock);
  printf ("(priority-donate-lower) acquire: got the lock\n");
  lock_release (lock);
  printf ("(priority-donate-lower) acquire: done\n");
}
