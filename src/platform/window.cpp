
#include <iostream>
#include <windows.h>

#include "platform/my_window.hpp"

#include "constants.hpp"


LRESULT CALLBACK my_window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    my_window* window = nullptr;

    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT* create = reinterpret_cast<CREATESTRUCT*>(lParam);
        window = static_cast<my_window*>(create->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
    } else {
        window = reinterpret_cast<my_window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
        if (window && window->to_render == true) {
            window->drawWireframe(hwnd, window->object.get_screen(), window->object.get_ndc_f(), RGB(255, 255, 255), lParam, uMsg);
        } 
        return 0;
    case WM_ERASEBKGND:
    return 1; 

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

bool my_window::create() {
    WNDCLASS wc = {};
    wc.lpfnWndProc = my_window::WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className.c_str();
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    if (!RegisterClass(&wc)) {
        MessageBox(nullptr, "Window Registration Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    hwnd = CreateWindow(
        className.c_str(),
        title.c_str(),
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        nullptr, nullptr, hInstance, this
    );

    if (!hwnd) {
        MessageBox(nullptr, "Window Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    return true;
}

void my_window::initDoubleBuffer(HWND hwnd) {
    HDC hdcWindow = GetDC(hwnd);
    hdcMem = CreateCompatibleDC(hdcWindow);
    hbmMem = CreateCompatibleBitmap(hdcWindow, width, height);
    hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);
    ReleaseDC(hwnd, hdcWindow);
}

void my_window::drawWireframe(HWND hwnd, std::vector<v2F>* vertices, std::vector<v3F>* faces, COLORREF color, LPARAM lParam ,  UINT uMsg) {
    my_window* window = nullptr;

    if (uMsg == WM_NCCREATE) {
        CREATESTRUCT* create = reinterpret_cast<CREATESTRUCT*>(lParam);
        window = static_cast<my_window*>(create->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
    } else {
        window = reinterpret_cast<my_window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (!window->hdcMem) {
        window->initDoubleBuffer(hwnd);
    }
    int height = 600;
    int width = 800;

    // Clear backbuffer with black (or your bg color)
    RECT rect = {0, 0, width, height};
    HBRUSH hbrBkGnd = CreateSolidBrush(RGB(0, 0, 0));
    FillRect(window->hdcMem, &rect, hbrBkGnd);
    DeleteObject(hbrBkGnd);

    // Create pen for drawing
    HPEN pen = CreatePen(PS_SOLID, 1, color);
    HPEN oldPen = (HPEN)SelectObject(window->hdcMem, pen);

    // Draw the wireframe on the backbuffer
    for (int i = 0; i < faces->size(); i++) {
        
        int i0 = (*faces)[i].x - 1;
        int i1 = (*faces)[i].y - 1;
        int i2 = (*faces)[i].z - 1;

        POINT p0 = { static_cast<LONG>((*vertices)[i0].x), static_cast<LONG>((*vertices)[i0].y) };
        POINT p1 = { static_cast<LONG>((*vertices)[i1].x), static_cast<LONG>((*vertices)[i1].y) };
        POINT p2 = { static_cast<LONG>((*vertices)[i2].x), static_cast<LONG>((*vertices)[i2].y) };

        MoveToEx(window->hdcMem, p0.x, p0.y, NULL);
        LineTo(window->hdcMem, p1.x, p1.y);

        MoveToEx(window->hdcMem, p1.x, p1.y, NULL);
        LineTo(window->hdcMem, p2.x, p2.y);

        MoveToEx(window->hdcMem, p2.x, p2.y, NULL);
        LineTo(window->hdcMem, p0.x, p0.y);
    }

    SelectObject(window->hdcMem, oldPen);
    DeleteObject(pen);

    // Now copy the backbuffer to the window
    HDC hdcWindow = GetDC(hwnd);
    BitBlt(hdcWindow, 0, 0, width, height, window->hdcMem, 0, 0, SRCCOPY);
    ReleaseDC(hwnd, hdcWindow);
}
