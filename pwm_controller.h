#ifndef ECE243_FINAL_PROJECT_MOTOR_SPEED_CONTROLLER_H
#define ECE243_FINAL_PROJECT_MOTOR_SPEED_CONTROLLER_H

/**** includes ****/
#include "gpio.h"


#define ANALOG_STEPS 255
#include <stdbool.h>
// frequency of the ARM A9 MPCore Timer on the DE1-SoC
#define A9_TIMER_FREQUENCY 200000000 // 200MHz

// addresses to the ARM A9 MPCore Timer registers
#define TIMER_LOAD_REGISTER 0xFFFEC600
#define TIMER_CONTROL_REGISTER 0xFFFEC608
#define TIMER_COUNTER_REGISTER 0xFFFEC604

/**** function prototypes ****/
// sets the countdown value according to the required frequency given in Hz
void configure_pwm_frequency(int frequency);

// generates a PWM signal at the specified gpio pin with the given "analog" average value (scaled)
void analog_write(int gpio_pin_address, int value);

void setup();

#endif //ECE243_FINAL_PROJECT_MOTOR_SPEED_CONTROLLER_H