cmake_minimum_required(VERSION 3.13)
project(ECE243_Final_Project C)

set(CMAKE_C_STANDARD 99)

add_executable(ECE243_Final_Project main.c vga_drawing_functions.c vga_drawing_functions.h line_follower.h line_follower.c gpio.c gpio.h pwm_controller.h pwm_controller.c FPGA_File.cpp)