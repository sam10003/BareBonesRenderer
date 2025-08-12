
#pragma once

#include <windows.h>
#include <string>

#include "utils/data_f.hpp"
#include "core/mesh.hpp"

class my_window {
    public:
        my_window(HINSTANCE hInstance, const std::string& title, int width, int height)
            : hInstance(hInstance), title(title), width(width), height(height), hwnd(nullptr) {}

        bool create();
        void run();

    private:
        HINSTANCE hInstance;
        HWND hwnd;
        std::string title;
        int width, height;
        const std::string className = "SimpleWindowClass";

        Mesh object;
        bool to_render = false;
        
        HDC hdcMem = nullptr;
        HBITMAP hbmMem = nullptr;
        HBITMAP hbmOld = nullptr;

        static void drawWireframe(HWND hwnd, std::vector<v2F>* vertices, std::vector<v3F>* faces, COLORREF color,LPARAM lParam , UINT uMsg);
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        void initDoubleBuffer(HWND hwnd);
};