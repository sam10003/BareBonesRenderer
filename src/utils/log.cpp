
#include <string>
#include <iostream>

#include "utils/log.hpp"


void info_dump::print_log() {
        std::cout << "\n"
                  << "Run_time: \n"
                  << " - Execution Loops: " << exec_loops << "\n"
                  << "File: \n"
                  << " - file_name: " << file_path << "\n"
                  << " - total vertex: " << v_n << "\n"
                  << " - total vertex(n): " << vn_n << "\n"
                  << " - total vertex(t): " << vt_n << "\n"
                  << " - total faces: " << f_n << "\n"
                  << "Boot: \n"
                  << " - starting position (camera): (" << start_cam_pos.x << "," << start_cam_pos.y << "," << start_cam_pos.z << ")\n"
                  << "User_Input: \n"
                  << " - W Pressed: " << pressed_W << "\n"
                  << " - A Pressed: " << pressed_A << "\n"
                  << " - S Pressed: " << pressed_S << "\n"
                  << " - D Pressed: " << pressed_D << "\n"
                  << " - L Pressed: " << pressed_L << "\n"
                  << " - R Pressed: " << pressed_R << "\n"
                  << " - No Input: " << no_input << "\n";
}