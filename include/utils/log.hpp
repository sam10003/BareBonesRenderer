
#pragma once

#include <string>
#include <iostream>

#include "utils/data_f.hpp"

typedef struct info_dump_t
{
    //execution
    long int exec_loops = 0;

    //file 
    std::string file_path = "";
    long int v_n = 0;
    long int vt_n = 0;
    long int vn_n = 0;
    long int f_n = 0;
    //boot
    v3F start_cam_pos;
    //user input
    long int pressed_W = 0;
    long int pressed_A = 0;
    long int pressed_S = 0;
    long int pressed_D = 0;
    long int pressed_L = 0;
    long int pressed_R = 0;
    long int no_input = 0;

    void print_log();

}info_dump;