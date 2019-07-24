#include "vga_drawing_functions.h"
#include "line_follower.h"
#include "pwm_controller.h"
#include "gpio.h"

/**** helper function prototypes ****/

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
