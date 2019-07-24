#define ANALOG_STEPS 255
#include <stdbool.h>
// frequency of the ARM A9 MPCore Timer on the DE1-SoC
#define A9_TIMER_FREQUENCY 200000000 // 200MHz

// addresses to the ARM A9 MPCore Timer registers
#define TIMER_LOAD_REGISTER 0xFFFEC600
#define TIMER_CONTROL_REGISTER 0xFFFEC608
#define TIMER_COUNTER_REGISTER 0xFFFEC604

void clear_screen();
void draw_line(int x0, int y0, int x1, int y1, short int color);
void plot_pixel(int x, int y, short int line_color);
int absolute(int y);
void wait_for_vsync();
void draw_screen(void);



void draw_screen_f();

volatile int *sw_addr = 0xFF200040;

volatile long int vga_update_counter = 200000000;
volatile int pixel_buffer_start; // global variable
volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
volatile int * back_buffer_address = (int *)0xFF203024;
volatile int * buffer_bit = (int *)0xFF203028;
volatile int * s_flag = (int *)0xFF20302C;
extern short MYIMAGE [240][320];
typedef struct rect{
    int x, y;
    int width;
    volatile short int color;
}rectangle;
rectangle rectangle_array[5];

/**** function prototypes ****/
// sets the countdown value according to the required frequency given in Hz
void configure_pwm_frequency(int frequency);

// generates a PWM signal at the specified gpio pin with the given "analog" average value (scaled)
void analog_write(int gpio_pin_address, int value);

void delay(int milliseconds);

int read_all(int sensor);
void move_circular();
bool read_gpio(int sensor);
void motor_enable(int motor, bool state);
void motor_control(int motor, bool state);
void gpio_setup();
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

//
void stop();

#include <stdbool.h>
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

/**** helper function prototypes ****/
void setup();


/**** main ****/
int main() {
    // this has to be called once
    setup();
    draw_screen_f();
    while(true) {
        //clear_screen();
        read_all(ALL_SENSOR);
        draw_screen();
        follow_path();
        //wait_for_vysnc();
        //pixel_buffer_start = *(pixel_ctrl_ptr + 1);
        //move_right();
    }
    return 0;
}

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


void follow_path() {
    volatile int motors_w = read_all(ALL_SENSOR) >> 15;
    //volatile int *LED_address  = (int*) 0xFF200000;
    //AND the sensor with the data reg to see if its ON or not

    //*LED_address = motors_w;
    // move_forward();
    /* if(motors == 131072){
         move_forward();
     }

     else if(motors == 32768 || motors == 65536){
         move_right();
    }
    else if (motors == 262144 || motors == 524288){
            move_left();
    }
    else if (motors == 0){
            stop();
    }*/

    volatile int sensor_array[5];
    volatile int j = 0;

    for(volatile int i = 0x1; i <= 0x10; i = i * 2){
        sensor_array[j] = motors_w & i;
        j++;
    }

    volatile int sensor_average = 0;
    volatile int sensor_sum = 0;
    volatile weight = -1000;

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
        draw_screen();
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



int read_all(int sensor){

    //if (vga_update_counter != 0) {
    //  vga_update_counter--;
    //}
    //else {
    //   vga_update_counter = 200000000;
    //int sw_read = *sw_addr;
    //sw_read = sw_read & 0x0000001F;
    //if (sw_read > 0){
    //draw_screen();
    // }
    //}

    // get the address of the data register
    volatile int *GPIO_data_reg = (int*) 0xFF200070;

    // read what is written in the gpio register
    int gpio_data_read = *GPIO_data_reg;

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



void draw_screen_f(void){

    // get the address of the DMA controller
    volatile int *pixel_ctrl_ptr = (int*) 0xFF203020;

    // read location of the pixel buffer from the pixel buffer controller
    pixel_buffer_start = *pixel_ctrl_ptr;

    // setting front pixel buffer to start of the FPGA on-chip memory
    *(pixel_ctrl_ptr + 1) = 0xC8000000; // first store the address in the back buffer

    // now, swap the front/back buffers, to set the front buffer location
    wait_for_vsync();

    // initialize a pointer to the pixel buffer, used by drawing functions
    pixel_buffer_start = *pixel_ctrl_ptr;

    // start with a blank screen
    clear_screen(); // pixel_buffer_start points to the pixel buffer

    // set back pixel buffer to start of SDRAM memory
    *(pixel_ctrl_ptr + 1) = 0xC0000000;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer

    // start with a blank screen on the back buffer
    clear_screen();






    //Rand the first location and direction of the rectangles
    volatile int i = 0;
    volatile int j = 0;
    volatile int k;
    // volatile int n = 5; //Total number of rectangles given to us

    //Loop through the rectangles and randomize them

    //Making an array of size 8 to hold rectangles





    rectangle_array[0].x = 42;
    rectangle_array[1].x = 92;
    rectangle_array[2].x = 147;
    rectangle_array[3].x = 202;
    rectangle_array[4].x = 257;



}
void draw_screen(void){
    clear_screen();
    volatile int i = 0;
    volatile int j = 0;
    volatile int k;
    volatile int n = 5;
    //AND the sensor with the data reg to see if its ON or not

    //*LED_address = motors_w;
    // volatile int n
    volatile int *LED_address  = (int*) 0xFF200000;
    volatile int motors_ww = read_all(ALL_SENSOR) >> 15;
    volatile int sensor_arrayy[5];
    //volatile int j = 0;

    for(volatile int i = 0x1; i <= 0x10; i = i * 2){
        sensor_arrayy[j] = motors_ww & i;
        j++;
    }



    for(i; i < n; i++){
        rectangle_array[i].width = 10;
        rectangle_array[i].y = 113;

        //rectangle_array[i].color = rand() % 0xFFFF;
    }
    if(sensor_arrayy[0]) {
        rectangle_array[0].color = 0xFFFF;
        *LED_address = 0x1;
    }
    else {
        rectangle_array[0].color = 0x0;
        *LED_address = 0x0;
    }

    if(sensor_arrayy[1]) {
        rectangle_array[1].color = 0xFFFF;
    }
    else {
        rectangle_array[1].color = 0x0;
    }

    if(sensor_arrayy[2]) {
        rectangle_array[2].color = 0xFFFF;
    }
    else {
        rectangle_array[2].color = 0x0;
    }

    if(sensor_arrayy[3]) {
        rectangle_array[3].color = 0xFFFF;
    }
    else {
        rectangle_array[3].color = 0x0;
    }

    if(sensor_arrayy[4]) {
        rectangle_array[4].color = 0xFFFF;
    }
    else {
        rectangle_array[4].color = 0x0;
    }

    // code for drawing the boxes and lines (not shown)
    //Go through each rectangle and draw the rectangle based on the width
    for (i =0; i < n; i++){

        for( j = 0 ; j < rectangle_array[i].width; j++){
            for( k = 0 ; k < rectangle_array[i].width; k++){
                plot_pixel(rectangle_array[i].x+k, rectangle_array[i].y+j, rectangle_array[i]. color );
            }
        }
    }




    //Wait for Vsync when we get that we know we can draw again
    //and we will have to put in the back buffer

    wait_for_vsync(); // swap front and back buffers on VGA vertical sync
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
}


// code for subroutines (not shown)

void wait_for_vsync(){
    //Write to frnt to swap cause we are swapping front with back always
    *pixel_ctrl_ptr = 1;
    while(((*s_flag) & 1) !=0){}//Loop until we get Vsync which means new frame

}


int y = 0;
//int goingDown = true;

// absolute value function
int abs(int x){

    if (x < 0)
        return (-x);
    return x;

}

// plot_pixel() subroutine
void plot_pixel(int x, int y, short int line_color)
{
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}

// clear_screen() subroutine
void clear_screen(){

    int x, y;
    for (x = 0; x < 320; x++){
        for (y = 0; y < 240; y++){
            plot_pixel(x, y, MYIMAGE[y][x]);
        }
    }

}

// draw_line() subroutine
void draw_line(int x0, int y0, int x1, int y1, short int colour){

    int is_steep;
    if (abs(y1 - y0) > abs(x1 - x0))
        is_steep = true;
    else
        is_steep = false;

    if (is_steep){

        // swap x0 and y0
        int temp = x0;
        x0 = y0;
        y0 = temp;

        // swap x1 and y1
        temp = x1;
        x1 = y1;
        y1 = temp;

    }

    if (x0 > x1){

        // swap x0 and x1
        int temp = x0;
        x0 = x1;
        x1 = temp;

        // swap y0 and y1
        temp = y0;
        y0 = y1;
        y1 = temp;

    }

    int deltax = x1 - x0;
    int deltay = abs( y1 - y0);
    int error = -(deltax/2);
    int y = y0;
    int y_step = 0;

    if (y0 < y1){
        y_step = 1;
    }
    else{
        y_step = -1;
    }

    int x;
    for (x = x0; x <= x1; x++){

        if (is_steep){
            plot_pixel(y, x, colour);
        }
        else{
            plot_pixel(x, y, colour);
        }

        error = error + deltay;

        if (error >= 0){
            y = y + y_step;
            error = error - deltax;
        }

    }

}

