
#pragma once

/**
 * this class hosts the rendering pipeline step by step
 * this way we can implement the pipeline to the run_loop more comfortably
 */

#include "utils/data_f.hpp"
#include "utils/log.hpp"

#include "core/mesh.hpp"
#include "core/camera.hpp"
#include "core/transform.hpp"

class Render{
    Camera cam; 
    Mesh &obj;

    //viewport values
    int height; 
    int width;

    //rendering values
    int farClip;
    float nearClip;

    public:
        Render(Mesh &obj,Camera cam);

        //setters & getters
        Camera get_cam();
        int get_width();
        int get_height();
        int get_farClip();
        float get_nearClip();

        void set_cam(Camera new_cam);
        void set_width(int new_width);
        void set_height(int new_height);
        void set_farClip(int new_farClip);
        void set_nearClip(float new_nearClip);

        //pipeline methods
        void transform(info_dump *log);
        void NDC(info_dump *log);
        void to_screen(info_dump *log);
        void reset(info_dump *log);
};