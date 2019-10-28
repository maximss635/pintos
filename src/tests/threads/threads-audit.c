/*
  File for 'threads-audit' task implementation.
*/

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/malloc.h"
#include "threads/thread.h"
#include "devices/timer.h"

static thread_func function;

void test_threads_audit(void) 
{
  thread_create("thread 1", 37, function, NULL);
  
  for (int i = 0; i < 10000; i++)
  for (int j = 0; j < 10000; j++)
  for (int k = 0; k < 10000; k++);

}

static void
function (void *aux UNUSED)
{
  msg ("%s run and go to sleep", thread_name());
  timer_sleep(20);
  msg ("%s wake up", thread_name());
}