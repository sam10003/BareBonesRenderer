#include <vector>

#include "core/render.hpp"
#include "core/camera.hpp"
#include "core/transform.hpp"
#include "core/mesh.hpp"

#include "utils/log.hpp"
#include "utils/data_f.hpp"

#include "constants.hpp"

Render::Render(Mesh &obj, Camera cam) : obj(obj), cam(cam) {
    //rendering values
    nearClip = Const::NEAR_CLIP;
    farClip = Const::FAR_CLIP;

    //viewport data
    width = Const::WIDTH;
    height = Const::HEIGHT;
}

Camera Render::get_cam(){
    return cam;
}
int Render::get_width(){
    return width;
}
int Render::get_height(){
    return height;
}
int Render::get_farClip(){
    return farClip;
}
float Render::get_nearClip(){
    return nearClip;
}

void Render::set_cam(Camera new_cam){
    cam = new_cam;
}
void Render::set_width(int new_width){
    width = new_width;
}
void Render::set_height(int new_height){
    height = new_height;
}
void Render::set_farClip(int new_farClip){
    farClip = new_farClip;
}
void Render::set_nearClip(float new_nearClip){
    nearClip = new_nearClip;
}

void Render::transform(info_dump *log){
    // within this function we carry out all the vertex transforms
    std::vector<v4F> *in = obj.get_v4();
    if (!in) return;
    std::vector<v4F> out = *in;

    // matrix creation
    M4x4 view = Transform::worldToView(cam);
    M4x4 pers = Transform::perspective(width, height, cam, farClip, nearClip);
    M4x4 transformation_matrix = pers * view;

    obj.set_transform_v4(Transform::apply(&out, transformation_matrix));
}

void Render::NDC(info_dump *log){
    //we carry the NDC set up here
    std::vector<v4F> *in = obj.get_transform_v4();
    obj.set_ndc_v(Transform::NDC(in,*obj.get_f(),*obj.get_ndc_f()));
    return;
}

void Render::to_screen(info_dump *log){
    //we map the coordinates to the screen here
    std::vector<v3F> *in = obj.get_ndc_v();
    obj.set_screen(Transform::NDC_to_screen(in,width,height));
    return;
}

void Render::reset(info_dump *log){
    obj.clean(log);
}