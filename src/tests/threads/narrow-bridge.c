
/* File for 'narrow_bridge' task implementation.  
   SPbSTU, IBKS, 2017 */

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/thread.h"
#include "threads/synch.h"
#include "narrow-bridge.h"

//my
#include "devices/timer.h"
#define MAX_VEHICLE 2
//end my

struct car car_init(enum car_priority prio, enum car_direction dir)
{
	struct car car;
	car.prio = prio;
	car.dir = dir;

	return car;
}

void line_init(struct line* car_line)
{
	car_line->cars_num = 0;
	cond_init(&car_line->cars);
}

static bool empty_line(struct line* line)
{
	return line->cars_num == 0;
}

static bool CanCarGo(struct car car)			//???
{
    if (cars_on_the_road == 0) 
	{
		if (car.prio == car_emergency)
			return 1;

		return (emer_passed < num_emergency_left + num_emergency_right ? 0 : 1);
	}
	else if ((cars_on_the_road < MAX_VEHICLE) && (current_direction == car.dir))
	{
		if (car.prio == car_emergency)
			return 1;

		unsigned int emer_passed = (car.dir == dir_left ? emer_passed_left : emer_passed_right);
		unsigned int num_emer = (car.dir == dir_left ? num_emergency_left : num_emergency_right);

		return (emer_passed < num_emer ? 0 : 1); 
	}     
	else
    {
	    return 0;
	}
}

static void get_in_line(struct car* car)
{
	enum car_priority prio = car->prio;
	enum car_direction dir = car->dir;

	struct line* line = &car_lines[prio][dir];

	line->cars_num++;
	cond_wait(&line->cars, &lock);
}

void narrow_bridge_init(unsigned int _num_emergency_left, unsigned int _num_emergency_right)
{
	num_emergency_left = _num_emergency_left;
	num_emergency_right = _num_emergency_right;

	cars_on_the_road = 0;

	emer_passed = 0;
	emer_passed_left = 0;
	emer_passed_right = 0;

	lock_init(&lock);
	for (int i = 0; i < 4; ++i)
		line_init(&car_lines[i / 2][i % 2]);
}

void arrive_bridge(enum car_priority prio, enum car_direction dir)
{
	lock_acquire(&lock);

	struct car currnet_car = car_init(prio, dir);

	while(!CanCarGo(currnet_car))
	{
		//printf("%s cannt go\n", thread_name());
		get_in_line(&currnet_car);
	}	//wait

	//went on the road
	cars_on_the_road++;
	current_direction = currnet_car.dir;
	if (currnet_car.prio == car_emergency)
	{
		emer_passed++;
		dir == dir_left ? emer_passed_left++ : emer_passed_right++;

		if (cars_on_the_road == 1)
			cond_signal(&car_lines[0][dir].cars, &lock);
	}

	lock_release(&lock);
}

void exit_bridge(enum car_priority prio UNUSED, enum car_direction dir)
{
	lock_acquire(&lock);

	cars_on_the_road--;

	struct line* line;

	if (!empty_line(&car_lines[1][1 - dir]))						//	(1 - dir) means the opposite dirrection
		line = &car_lines[1][1 - dir];

	else if (!empty_line(&car_lines[1][dir]))
		line = &car_lines[1][dir];
	
	else if (!empty_line(&car_lines[0][1 - dir]))
		line = &car_lines[0][1 - dir];
	
	else
		line = &car_lines[0][dir];
	

	line->cars_num--;
	cond_signal(&line->cars, &lock);

	lock_release(&lock);
}
