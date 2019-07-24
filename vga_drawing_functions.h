#ifndef ECE243_FINAL_PROJECT_VGA_DRAWING_FUNCTIONS_H
#define ECE243_FINAL_PROJECT_VGA_DRAWING_FUNCTIONS_H

#include <stdlib.h>

void clear_screen();
void draw_line(int x0, int y0, int x1, int y1, short int color);
void plot_pixel(int x, int y, short int line_color);
int absolute(int y);
void wait_for_vsync();
void draw_screen(void);
void draw_screen_f(void);
#endif //ECE243_FINAL_PROJECT_LINE_FOLLOWER_H