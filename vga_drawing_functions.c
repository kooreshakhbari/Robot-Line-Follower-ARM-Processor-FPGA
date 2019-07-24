#include "gpio.h"
#include"vga_drawing_functions.h"

volatile int pixel_buffer_start; // global variable
volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
volatile int * back_buffer_address = (int *)0xFF203024;
volatile int * buffer_bit = (int *)0xFF203028;
volatile int * s_flag = (int *)0xFF20302C;
short MYIMAGE [240][320];
typedef struct rect{
    int x, y;
    int width;
    short int color;
}rectangle;

rectangle rectangle_array[5];

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

