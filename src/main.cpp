
#include <windows.h>
#include <iostream>

#include "platform/my_window.hpp"
#include "constants.hpp"


//window creation
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    my_window myWindow(hInstance, "Wireframe renderer", Const::WIDTH, Const::HEIGHT);
    
    if (!myWindow.create()) {
        std::cout << ".[window_creation] failed\n";

        std::cin.get(); //await user input
        std::cin.get();
        return -1;
    }
    myWindow.run(); //run loop
    return 0;
}