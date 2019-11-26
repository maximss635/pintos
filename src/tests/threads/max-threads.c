/*
  File for 'max-threads' task implementation.
*/

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/malloc.h"
#include "threads/thread.h"

static thread_func f;

void test_max_threads(void) 
{
  for (int i = 0; i < 10; ++i)
  {
    char name[] = "00001";
    thread_create("a", 1, f, NULL);

    name[4]++;
    for (int j = 4; j > 0; ++j)
    {
      if (name[j] > '9')
      {
        name[j - 1]++;
        name[j] = '0';
      }
    }
  }

  SHOW_ALL_LIST();
}

static void f (void* aux UNUSED)
{
  static int cnt = 1;
  //if (cnt % 100000 == 0)
    //msg("%d thread is active", cnt);
  cnt++;
}
