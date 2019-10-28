/*
  File for 'max-rec-calls' task implementation.
*/

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/malloc.h"
#include "threads/thread.h"

void test_max_rec_calls(void) 
{
  static int k = 1;
  msg("call number %d", k);
  k++;
  test_max_rec_calls();
}
