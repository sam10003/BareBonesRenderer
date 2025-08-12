
#pragma once

/**
 * Camera class, the class mainly stores the values and vectors that compose the camera
 * it also contains the logic for the rotation of said camera:
 * 
 * the camera uses an initial center 0,0,0 as reference and can only be rotated by yaw and pitch (up vector is constant)
 */

#include "utils/data_f.hpp"

class Camera
{
    v3F pos; //or eye
    v3F center; //reference
    v3F up;
    float fov; //radians
    
    //rotation
    float yaw;
    float pitch;

    public:
        Camera(v3F pos);

        //getters & setters
        v3F getPos();
        void setPos(v3F newPos);
        v3F getCenter();
        void setCenter(v3F new_center);
        v3F getUp();
        float getFOV();

        //function will rotate the camera based on the delta values (keeping roll constant)
        void rotate(float deltaYaw, float deltaPitch);

        //compute the new center based on yaw and pitch
        void updateCenter();
};