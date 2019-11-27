
/* Helper functions for 'narrow_bridge' task.
   SPbSTU, IBKS, 2017 */

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/thread.h"
#include "threads/synch.h"
#include "threads/interrupt.h"
#include "devices/timer.h"
#include <list.h>
#include "narrow-bridge.h"


void create_vehicles(unsigned int count, thread_func* func);
void thread_normal_left(void* arg UNUSED);
void thread_normal_right(void* arg UNUSED);
void thread_emergency_left(void* arg UNUSED);
void thread_emergency_right(void* arg UNUSED);
unsigned int threads_count(void);
void threads_counter(struct thread *t UNUSED, void *aux);
void wait_threads(void);
void one_vehicle(struct car car);

static unsigned int threads_count_on_start = 0;

//my
static struct car 
car(enum car_priority prio, enum car_direction dir)
{
	struct car car = {prio, dir};

	return car;
}

static void
print_info (unsigned int num_vehicles_left, unsigned int num_vehicles_right,
        unsigned int num_emergency_left, unsigned int num_emergency_right)
{
    int i = 1, sum = 1;
    //SHOW_ALL_LIST();
    printf("Norm: \n\t%d left  ", num_vehicles_left);
    printf("(");
    sum += num_vehicles_left;
    while (i != sum)
    {
        printf("%d ", i);
        i++;
    }
    printf(")");

    printf("\n\t%d right ", num_vehicles_right);   
    printf("(");
    sum += num_vehicles_right;
    while (i != sum)
    {
        printf("%d ", i);
        i++;
    }
    printf(")");

    printf("\nEmer: \n\t%d left  ", num_emergency_left);
    printf("(");
    sum += num_emergency_left;
    while (i != sum)
    {
        printf("%d ", i);
        i++;
    }
    printf(")");
    
    printf("\n\t%d right ", num_emergency_right);       
    printf("(");
    sum += num_emergency_right;
    while (i != sum)
    {
        printf("%d ", i);
        i++;
    }
    printf(")\n");   
}
//end my

void test_narrow_bridge(unsigned int num_vehicles_left, unsigned int num_vehicles_right,
        unsigned int num_emergency_left, unsigned int num_emergency_right)
{
    threads_count_on_start = threads_count();

    narrow_bridge_init(num_emergency_left, num_emergency_right);

    create_vehicles(num_vehicles_left, thread_normal_left);
    create_vehicles(num_vehicles_right, thread_normal_right);
    create_vehicles(num_emergency_left, thread_emergency_left);
    create_vehicles(num_emergency_right, thread_emergency_right);
    
    //my
    print_info(num_vehicles_left, num_vehicles_right, num_emergency_left, num_emergency_right);
    //end my

    int start_time_for_cars = timer_ticks();
    wait_threads();

    //my
    printf("\nTime for cars: %d, ", timer_elapsed(start_time_for_cars));
    printf("number of cars: %d\n\n", num_vehicles_left + num_vehicles_right + num_emergency_left + num_emergency_right);
    //end my
}

void threads_counter(struct thread *t UNUSED, void *aux)
{
   unsigned int* pcnt = (aux);
   (*pcnt)++;
}

unsigned int threads_count(void)
{
    unsigned int cnt = 0;
    enum intr_level old_level = intr_disable();
    thread_foreach(threads_counter, &cnt);
    intr_set_level (old_level);
    return cnt;
}
  
void wait_threads()
{
  while(1)
  {
    if (threads_count() > threads_count_on_start)
      thread_yield();
    else
      return;
  }
}

void create_vehicles(unsigned int count, thread_func* func)
{
     unsigned int i;
     static unsigned int car_id = 0;
     for(i = 0; i < count; i++)
     {
         char name[16];
         snprintf(name, sizeof(name), "%u", ++car_id);
         thread_create(name, PRI_DEFAULT, func, 0);
     }
}

void thread_normal_left(void* arg UNUSED)
{
    one_vehicle(car(car_normal, dir_left));
}

void thread_normal_right(void* arg UNUSED)
{
    one_vehicle(car(car_normal, dir_right));
}

void thread_emergency_left(void* arg UNUSED)
{
    one_vehicle(car(car_emergency, dir_left));
}

void thread_emergency_right(void* arg UNUSED)
{
    one_vehicle(car(car_emergency, dir_right));
}

void one_vehicle(struct car car)
{
    arrive_bridge(car);
    cross_bridge(car);
    exit_bridge();
}
                                                                
void cross_bridge(struct car car)
{
    //my
    static int64_t last_timer_ticks = 0;
    if (timer_ticks() != last_timer_ticks)
        printf("\n");
    last_timer_ticks = timer_ticks();
    //end my
    
    msg("Vehicle: %4s, prio: %s, direct: %s, ticks=%4llu", 
          thread_current()->name, 
          car.prio == car_emergency ? "emer" : "norm",
          car.dir == dir_left ? "l -> r" : "l <- r",
          (unsigned long long) timer_ticks ());
    timer_sleep(10);
}
