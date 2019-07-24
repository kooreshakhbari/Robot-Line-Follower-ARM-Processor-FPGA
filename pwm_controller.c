#include "pwm_controller.h"

void setup() {
    gpio_setup();
    configure_pwm_frequency(490); // 490Hz
}


/**** global variables ****/
// countdown value for the ARM A9 MPCore Timer on the DE1-SoC
int TIMER_COUNTDOWN_VALUE = 0;


/**** function definitions ****/

// sets the countdown value according to the required frequency given in Hz
void configure_pwm_frequency(int frequency) {

    // update the value of the global timer countdown value
    // formula: countdown value = f_A9_Timer/f_input
    TIMER_COUNTDOWN_VALUE = A9_TIMER_FREQUENCY/frequency;

    // set the countdown value in the timer load register
    *(int*) TIMER_LOAD_REGISTER = TIMER_COUNTDOWN_VALUE;

    // write to the timer control register to start the timer
    *(int*) TIMER_CONTROL_REGISTER = 0b11; // set bits: A = 1 (auto-reload), E = 1 (enable)
}

// generates a PWM signal at the specified gpio pin with the given "analog" average value (scaled)
void analog_write(int pin_address, int value) {

    // get the count value from the register
    volatile int current_count = *(int*) TIMER_COUNTER_REGISTER;

    // if count < value * (COUNTDOWN_VALUE/STEP_VALUE)
    if (current_count < value * (TIMER_COUNTDOWN_VALUE/ANALOG_STEPS)) {
        // write HIGH to the pin
        motor_enable(pin_address, HIGH);
    }
    else {
        // write LOW to the pin
        motor_enable(pin_address, LOW);
    }
}

