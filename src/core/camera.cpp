
#include <cmath>

#include "utils/data_f.hpp"

#include "core/camera.hpp"

#include "constants.hpp"


Camera::Camera(v3F pos)
{
    //initial values
    this->pos = pos; 
    center = v3F(0,0,0);
    up = v3F(0,1,0);
    fov = Const::FOV;

    //direction vector
    v3F direction;
    direction.x = center.x - pos.x;
    direction.y = center.y - pos.y;
    direction.z = center.z - pos.z;

    //compute yaw and pitch
    yaw = atan2f(direction.x, direction.z);
    pitch = asinf(direction.y);

    updateCenter();
}

//setters & getters
v3F Camera::getPos(){
    return pos;
}
void Camera::setPos(v3F newPos){
    pos = newPos;
}
v3F Camera::getCenter(){
    return center;
}
void Camera::setCenter(v3F new_center){
    center = new_center;
}
v3F Camera::getUp(){
    return up;
}
float Camera::getFOV(){
    return fov;
}

void Camera::rotate(float deltaYaw, float deltaPitch)
{
    //rotating the camera taking into account the said PITCH LIMIT

    yaw += deltaYaw;
    pitch += deltaPitch;

    if (pitch > Const::PITCH_LIMIT) pitch = Const::PITCH_LIMIT;
    if (pitch < -Const::PITCH_LIMIT) pitch = -Const::PITCH_LIMIT;

    updateCenter();  //recompute center from yaw/pitch
}

void Camera::updateCenter()
{
    //compute the new center
    v3F direction;
    direction.x = cosf(pitch) * sinf(yaw);
    direction.y = sinf(pitch);
    direction.z = cosf(pitch) * cosf(yaw);

    center = { pos.x + direction.x, pos.y + direction.y, pos.z + direction.z };
}