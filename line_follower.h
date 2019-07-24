#ifndef ECE243_FINAL_PROJECT_LINE_FOLLOWER_H
#define ECE243_FINAL_PROJECT_LINE_FOLLOWER_H


/**** includes ****/
#include "pwm_controller.h"

/**** defines ****/
#define FORWARD_SPEED 75
#define RIGHT_SPEED 85
#define LEFT_SPEED 85
#define OFF 0
#define LEFT_DELAY 20
#define RIGHT_DELAY 20
#define FORWARD_DELAY 20


/**** function prototypes ****/
//
void follow_path();

//
void move_forward();

//
void move_right();

//
void power_right();

//
void move_left();

//
void power_left();


void move_circular();
//
void stop();


#endif //ECE243_FINAL_PROJECT_LINE_FOLLOWER_H

