#include "line_follower.h"



void follow_path() {
    volatile int motors_w = read_all(ALL_SENSOR) >> 15;
    volatile int *LED_address  = (int*) 0xFF200000;
    //AND the sensor with the data reg to see if its ON or not

    *LED_address = motors_w;


    volatile int sensor_array[5];
    volatile int j = 0;

    for(volatile int i = 0x1; i <= 0x10; i = i * 2){
        sensor_array[j] = motors_w & i;
        j++;
    }

    volatile int sensor_average = 0;
    volatile int sensor_sum = 0;
    volatile int weight = -1000;

    for (volatile int i = 0; i <= 4; i++){
        sensor_average += sensor_array[i] * i * weight;   //weighted mean
        sensor_sum += sensor_array[i];
        weight += 500;
    }

    if (sensor_average > 0 ){
        move_left();
    }
    else if (sensor_average < 0){
        move_right();
    }
    else if (sensor_average == 0 && sensor_sum == 0){
        move_circular();
        //stop();
    }
    else{
        move_forward();
    }




}

//
void move_forward() {
    motor_control(FORWARD_R, HIGH);
    analog_write(ENABLE_R, 120);
    motor_control(FORWARD_L, HIGH);
    analog_write(ENABLE_L, 120);
}

//
void move_right() {
    analog_write(ENABLE_R, 0);
    motor_control(FORWARD_L, HIGH);
    analog_write(ENABLE_L, 75);
}

//
void power_right() {
    //
}

//
void move_left() {
    // turn motors on properly
    analog_write(ENABLE_L, 0);
    motor_control(FORWARD_R, HIGH);
    analog_write(ENABLE_R, 75);

}

void move_circular(){
    analog_write(ENABLE_L, 0);
    analog_write(ENABLE_R, 0);
    motor_control(FORWARD_R, HIGH);
    analog_write(ENABLE_R, 75);
    motor_control(REVERSE_L, HIGH);
    analog_write(ENABLE_L, 75);

}

//
void power_left() {
//    analog_write();
}

//
void stop() {
    analog_write(ENABLE_L, 0);
    analog_write(ENABLE_R, 0);
}


