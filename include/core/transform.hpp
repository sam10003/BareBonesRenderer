
#pragma once

/**
 * The transform class contains various math/algorithm heavy functions that carry out the rendering
 * all the functions within this class are static (excluding the helper functions they may have)
 * for that reason the only class which applies them is the rendering class which carries out the pipeline
 */

#include <vector>

#include "utils/data_f.hpp"
#include "core/camera.hpp"


class Transform{
    public:
        static M4x4 worldToView(Camera cam); //creates a view matrix
        static M4x4 perspective(int width, int height, Camera cam, float f, float n); //creates a perspective matrix
        static std::vector<v4F>* apply(std::vector<v4F>* world_space, M4x4 matrix); //applies said matrix

        //carries out the NDC and the clipping of geometry outside the frustrum
        static std::vector<v3F>* NDC(std::vector<v4F>* clip_space, std::vector<v3F> &fin, std::vector<v3F> &fout);
        
        //converts the NDC into screen coordinates
        static std::vector<v2F> *NDC_to_screen(std::vector<v3F> *in,int width,int height);
};