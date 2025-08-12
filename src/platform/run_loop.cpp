
#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

#include "platform/my_window.hpp"

#include "utils/data_f.hpp"
#include "utils/log.hpp"

#include "core/mesh.hpp"
#include "core/camera.hpp"
#include "core/transform.hpp"
#include "core/render.hpp"

#include "constants.hpp"


char input(info_dump *log); //will be ran once per loop
v3F get_camera_origin();


void my_window::run() {
    MSG msg = {};
    info_dump *log = new info_dump;
    //this is the setup area that will run once

    //basic file input zone:
    //list the file options
    std::cout << "current available files (within assets)\n";
    std::string assets_path = "../assets";
    try {
        for (const auto& entry : std::filesystem::directory_iterator(assets_path)) {
            if (!entry.is_directory()) {
                std::cout << " - " << entry.path().filename().string() << "\n";
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error reading assets directory: " << e.what() << "\n";
    }
    //we introduce the desired file
    std::cout << "please introduce the name of the .obj file: ";
    std::string f_name;
    std::cin >> f_name;
    f_name = "../assets/" + f_name;
    log->file_path = f_name;

    //file reading
    std::cout << ">opening file...\n";
    std::fstream fi;
    fi.open(f_name,std::ios::in);
    if(!fi.is_open()){
        std::cerr << ".error opening file " << f_name << "\n";
        log->print_log();
        delete log;
        
        std::cin.get(); //wait for user input
        std::cin.get();
        return;
    }

    //we load the mesh
    std::cout << "::parsing...\n";
    object.open_obj(log,fi);
    object.to_v4();
    std::cout << "::parsed succesfully:" << (log->v_n + log->vt_n + log->vn_n + log->f_n) << " elements!\n";
    
    //lets get the camera values
    std::cout << "camera position(xyz):\n";
    v3F camera_origin;
    do {
        camera_origin = get_camera_origin();
    } while(camera_origin.x == 0 && camera_origin.y == 0 && camera_origin.z == 0);
    log->start_cam_pos = camera_origin;
    Camera cam(camera_origin);

    //camera movement values
    bool drag = false;
    POINT prev_cursor;
    prev_cursor.x = 0;
    prev_cursor.y = 0;

    //pipeline
    Render rend(object,cam);

    HDC hdcWindow = GetDC(hwnd);
    hdcMem = CreateCompatibleDC(hdcWindow);
    hbmMem = CreateCompatibleBitmap(hdcWindow, width, height);
    hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);
    ReleaseDC(hwnd, hdcWindow);
    while (true) {
        log->exec_loops += 1;
        SetCursor(LoadCursor(NULL, IDC_ARROW)); //visual things
        //message handling

        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT) { //closing 
                log->print_log();
                delete log;
                fi.close();
                
                std::cout << "closing...";
                std::cin.get();
                std::cin.get();
                break;
            }
        }

        //user input handling
        char in = input(log);

        //movement
        float speed = Const::SPEED;

        v3F forward = normalize(cam.getCenter() - cam.getPos());
        v3F up = cam.getUp();  
        v3F right = normalize(cross(forward, up));
        switch(in)
        {
            case 'W': {
                v3F newPos = cam.getPos() + forward * speed;
                cam.setPos(newPos);
                cam.setCenter(newPos + forward);
                drag = false;
                break;
            }
            case 'S': {
                v3F newPos = cam.getPos() - forward * speed;
                cam.setPos(newPos);
                cam.setCenter(newPos + forward);
                drag = false;
                break;
            }
            case 'A': {
                v3F newPos = cam.getPos() - right * speed;
                cam.setPos(newPos);
                cam.setCenter(newPos + forward);
                drag = false;
                break;
            }
            case 'D': {
                v3F newPos = cam.getPos() + right * speed;
                cam.setPos(newPos);
                cam.setCenter(newPos + forward);
                drag = false;
                break;
            }
            case 'L':
                drag = false;
            break;
            case 'R':{
                    POINT cur_cursor;
                    GetCursorPos(&cur_cursor);
                    int dx = prev_cursor.x - cur_cursor.x;
                    int dy = prev_cursor.y - cur_cursor.y;

                    if(drag != false){
                        float sensitivity = Const::SENSITIVITY;
                        cam.rotate(dx * sensitivity, dy * sensitivity);
                    } else {
                        drag = true;
                    }
                    prev_cursor = cur_cursor;
            }
            break;
            case 'O':
                drag = false;
            break;
        }

        rend.set_cam(cam);
        rend.transform(log); 
        rend.NDC(log); 
        rend.to_screen(log); 

        to_render = true;
        InvalidateRect(hwnd, NULL, TRUE); 
        UpdateWindow(hwnd);
        to_render = false;
        rend.reset(log);

        //to prevent CPU overloading
        Sleep(10);
    }
}


char input(info_dump *log)
{
    //this function converts the user input to characters
    // -W-A-S-D- -L=left click -R=right click
    if (GetAsyncKeyState('W') & 0x8000) {log->pressed_W += 1;   return 'W';}
    if (GetAsyncKeyState('A') & 0x8000) {log->pressed_A+= 1;   return 'A';}
    if (GetAsyncKeyState('S') & 0x8000) {log->pressed_S += 1;   return 'S';}
    if (GetAsyncKeyState('D') & 0x8000) {log->pressed_D += 1;   return 'D';}
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {log->pressed_L += 1;    return 'L';}
    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {log->pressed_R += 1;    return 'R';}
    log->no_input += 1;
    return 'O';
}

v3F get_camera_origin()
{
    //this function is pretty straight foward like
    //we ask for 3 floats and we pack them up in a struct home
    float v[3];
    for (int i = 0; i < 3; i++) {
        std::cin >> v[i];
        if (v[i] < -1e5 || v[i] > 1e5) {
            std::cout << ".[camera origin] invalid input \n";
            return v3F(0,0,0);
        }
    }


    v3F to_v(v[0],v[1],v[2]);
    return to_v;
}

