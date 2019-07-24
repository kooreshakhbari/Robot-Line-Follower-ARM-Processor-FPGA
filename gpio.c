#include <stdbool.h>
#include "gpio.h"
#include "vga_drawing_functions.h"



int read_all(int sensor){

    // get the address of the data register
    volatile int *GPIO_data_reg = (int*) 0xFF200070;

    // read what is written in the gpio register
    int gpio_data_read = *GPIO_data_reg;
    draw_screen();
    // get the address of the direction register
    volatile int *GPIO_dir_reg = (int*) 0xFF200074;

    //Led address
    volatile int *LED_address  = (int*) 0xFF200000;

    //AND the sensor with the data reg to see if its ON or not

    //*LED_address = (gpio_data_read & sensor) >> 15;
    volatile int number = gpio_data_read & sensor;
    return number;
}



bool read_gpio(int sensor){

    // get the address of the data register
    volatile int *GPIO_data_reg = (int*) 0xFF200070;

    // read what is written in the gpio register
    int gpio_data_read = *GPIO_data_reg;

    // get the address of the direction register
    volatile int *GPIO_dir_reg = (int*) 0xFF200074;

    //Led address
    volatile int *LED_address  = (int*) 0xFF200000;

    if(gpio_data_read & sensor == 0x0){
        return LOW;
    }
    else {
        return HIGH;
    }

}
void motor_enable(int motor, bool state){
    // get the address of the data register
    volatile int *GPIO_data_reg = (int*) 0xFF200070;

    // read what is written in the gpio register
    int gpio_data_read = *GPIO_data_reg;

    // get the address of the direction register
    volatile int *GPIO_dir_reg = (int*) 0xFF200074;

    //Check if we are enabling

    if(state == HIGH){
        *GPIO_data_reg = gpio_data_read | motor;
    }
        //we are turning off enable
    else{
        *GPIO_data_reg = (gpio_data_read & (~motor));

    }
}

void motor_control(int motor, bool state){
    // get the address of the data register
    volatile int *GPIO_data_reg = (int*) 0xFF200070;

    // read what is written in the gpio register
    int gpio_data_read = *GPIO_data_reg;

    // get the address of the direction register
    volatile int *GPIO_dir_reg = (int*) 0xFF200074;

    //Check if we are turning on or off
    if(state == HIGH){
        if(motor == FORWARD_R){
            //First invert the reverse B for A then Turn on Forward
            gpio_data_read = gpio_data_read & (~REVERSE_R);
            *GPIO_data_reg = gpio_data_read | motor;
        }
        else if(motor == REVERSE_R){
            //First invert the reverse B for A then Turn on Forward
            gpio_data_read = gpio_data_read & (~FORWARD_R);
            *GPIO_data_reg = gpio_data_read | motor;
        }

        else if(motor == FORWARD_L){
            //First invert the reverse B for A then Turn on Forward
            gpio_data_read = gpio_data_read & (~REVERSE_L);
            *GPIO_data_reg = gpio_data_read | motor;
        }

        else{
            //First invert the reverse B for A then Turn on Forward
            gpio_data_read = gpio_data_read & (~FORWARD_L);
            *GPIO_data_reg = gpio_data_read | motor;
        }


    }
        //we are turning off enable
    else{
        *GPIO_data_reg = (gpio_data_read & (~motor));

    }
}



void gpio_setup(){
    // get the address of the data register
    volatile int *GPIO_data_reg = (int*) 0xFF200070;

    // read what is written in the gpio register
    int gpio_data_read = *GPIO_data_reg;

    // get the address of the direction register
    volatile int *GPIO_dir_reg = (int*) 0xFF200074;

    //Led address
    volatile int *LED_address  = (int*) 0xFF200000;

    //Setting D15 - D19 as inputs for the sensors and everything else as outputs
    *GPIO_dir_reg = 0xFFF07FFF;
    *GPIO_data_reg = 0xFC000000;
    return;
}

