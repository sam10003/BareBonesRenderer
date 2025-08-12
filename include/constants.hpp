
#pragma once

//here live all constants
namespace Const{
    // window dimensions
    const int HEIGHT = 600;
    const int WIDTH = 800;

    //math
    const float PI = 3.14159f;

    //camera
    const float FOV = 90.0f;
    const float PITCH_LIMIT = 89.0f * (PI / 180.0f); // 89 degrees in radians

    const float SPEED = 0.1f;
    const float SENSITIVITY = 0.005f;

    //rendering
    const float NEAR_CLIP = 0.01f;
    const float FAR_CLIP = 1000.0f;
}