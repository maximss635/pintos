
/* File for 'narrow_bridge' task implementation.  
   SPbSTU, IBKS, 2017 */

#ifndef __narrow_bridge_h
#define __narrow_bridge_h

enum car_priority
{
   car_normal = 0,     // Low priority car
   car_emergency = 1    // High priority car
};

enum car_direction
{
   dir_left = 0,        // Going from left -> right
   dir_right = 1        // Going from right -> left
};

//my
struct car {
   enum car_priority prio;
   enum car_direction dir;
};
//end my

void narrow_bridge_init(unsigned int num_emergency_left, unsigned int num_emergency_right);
void arrive_bridge(struct car current_car);
void exit_bridge(void);

void cross_bridge(struct car car);

void test_narrow_bridge(unsigned int num_vehicles_left, unsigned int num_vehicles_right,
        unsigned int num_emergency_left, unsigned int num_emergency_right);

//my
struct line {
   struct condition cars;
   unsigned int cars_num;
};

void line_init(struct line* car_line);

unsigned int num_emergency_left;
unsigned int num_emergency_right;

unsigned int cars_on_the_road;

unsigned int emer_passed;
unsigned int emer_passed_left;
unsigned int emer_passed_right;

enum car_direction current_direction;

struct lock lock;
struct line car_lines[2][2];

//end my


#endif // __narrow_bridge_h
