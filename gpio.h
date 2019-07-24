#ifndef ECE243_FINAL_PROJECT_GPIO_H
#define ECE243_FINAL_PROJECT_GPIO_H


#include <stdbool.h>

int read_all(int sensor);
bool read_gpio(int sensor);
void motor_enable(int motor, bool state);
void motor_control(int motor, bool state);
void gpio_setup();

#define SENSOR_0 0x8000
#define SENSOR_1 0x10000
#define SENSOR_2 0x20000
#define SENSOR_3 0x40000
#define SENSOR_4 0x80000

#define ENABLE_L    0X1
#define FORWARD_L   0X2
#define REVERSE_L   0X4
#define OFF_R       0x9FFFFFFF



#define ENABLE_R    0X8
#define FORWARD_R   0X10
#define REVERSE_R   0X20
#define OFF_L       0xF3FFFFFF


#define ALL_SENSOR 0xF8000

#define HIGH true
#define LOW false

#endif //ECE243_FINAL_PROJECT_GPIO_H