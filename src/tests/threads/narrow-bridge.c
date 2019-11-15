
/* File for 'narrow_bridge' task implementation.  
   SPbSTU, IBKS, 2017 */

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/thread.h"
#include "threads/synch.h"
#include "narrow-bridge.h"

//my
#include "devices/timer.h"
//end my

// Called before test. Can initialize some synchronization objects.
void narrow_bridge_init(void)
{
	sema_init(&sema, 2);
	number_of_car = 0;
}

inline int choose_priority (enum car_priority prio, enum car_direction dir)
{
	int p = 51;
	if (prio == car_emergency)
		p += 2;
	if (dir == dir_left)
		p++;
	return p;
}

void arrive_bridge(enum car_priority prio, enum car_direction dir)
{
	number_of_car++;
	printf("%d) arrive bridge\n", number_of_car);
	if (prio == car_normal)
	{
		printf("\tdelay\n");
		timer_sleep(5);
	}

	thread_set_priority(choose_priority(prio, dir));
	sema_down(&sema);
}

void exit_bridge(enum car_priority prio, enum car_direction dir)
{
	sema_up(&sema);
}
