
#include <cmath>
#include <algorithm>
#include <iostream>

#include "core/transform.hpp"
#include "utils/data_f.hpp"
#include "core/camera.hpp"

//helper functions for the NDC
bool inside(const v4F& v, int plane);
v4F interpolate(const v4F& a, const v4F& b, int plane);
std::vector<v4F> clipAgainstPlane(const std::vector<v4F>& poly, int plane);
std::vector<v4F> clipTriangle(std::vector<v4F> tri);

//matrix building
M4x4 Transform::worldToView(Camera cam){
    //for this we need to first form the camera basis
    //let phi be -z | let rho be x | let mu be y
    v3F phi,rho,mu;
    phi = normalize(cam.getCenter() - cam.getPos());
    rho = normalize(cross(phi,cam.getUp()));
    mu = cross(rho,phi);

    //expressions we would rather not compute 20 billion times
    float w1 = -1 * dot(rho, cam.getPos());
    float w2 = -1 * dot(mu, cam.getPos());
    float w3 = dot(phi, cam.getPos());

    //we create the matrix
    M4x4 view_matrix;
    view_matrix.m[0][0] = rho.x;  view_matrix.m[0][1] = rho.y;  view_matrix.m[0][2] = rho.z;  view_matrix.m[0][3] = w1;
    view_matrix.m[1][0] = mu.x;   view_matrix.m[1][1] = mu.y;   view_matrix.m[1][2] = mu.z;   view_matrix.m[1][3] = w2;
    view_matrix.m[2][0] = -phi.x; view_matrix.m[2][1] = -phi.y; view_matrix.m[2][2] = -phi.z; view_matrix.m[2][3] = w3;
    view_matrix.m[3][0] = 0;      view_matrix.m[3][1] = 0;      view_matrix.m[3][2] = 0;      view_matrix.m[3][3] = 1;
    return view_matrix;
}
M4x4 Transform::perspective(int width, int height, Camera cam, float f, float n){
    //this function builds the perspective matrix

    /* Parameters
        -width/heigh: data of the corresponding viewport
        -cam: corresponding camera object
        -f: far clipping distance
        -n: near clipping distance
    */

    //matrix value preparations
    float ratio = (float)width / (float)height;
    float halfFov = cam.getFOV() / 2.0f; 
    
    //expressions we precompute
    float w1 = 1.0f / (std::tan(halfFov) * ratio);
    float w2 = 1.0f / std::tan(halfFov);
    float w3 = -1 * ((f + n) / (f - n));
    float w4 = -1 * ((2.0f * f * n) / (f - n));

    //we create the matrix
    M4x4 pers_matrix;
    pers_matrix.m[0][0] = w1; pers_matrix.m[0][1] = 0;  pers_matrix.m[0][2] = 0;  pers_matrix.m[0][3] = 0;
    pers_matrix.m[1][0] = 0;  pers_matrix.m[1][1] = w2; pers_matrix.m[1][2] = 0;  pers_matrix.m[1][3] = 0;
    pers_matrix.m[2][0] = 0;  pers_matrix.m[2][1] = 0;  pers_matrix.m[2][2] = w3; pers_matrix.m[2][3] = w4;
    pers_matrix.m[3][0] = 0;  pers_matrix.m[3][1] = 0;  pers_matrix.m[3][2] = -1; pers_matrix.m[3][3] = 0;
    return pers_matrix;
}

std::vector<v4F>* Transform::apply(std::vector<v4F>* world_space, M4x4 matrix){

    //essentially we apply the linear transformation to every vertex
    std::vector<v4F>* out = new std::vector<v4F>();
    for(int i=0;i<world_space->size();i++){
        out->push_back(mat4_multiply_vec4(matrix,(*world_space)[i]));
    }
    return out;
}

//NDC + clipping
std::vector<v3F>* Transform::NDC(std::vector<v4F>* clip_space, std::vector<v3F>& fin, std::vector<v3F>& fout) {
    //this function will clip all the corresponding geometry based on the Cohen–Sutherland algorithm
    //afterwards it applies the NDC transform

    /* Parameters
        -clip_space: array of vertex post view/perspective transformation
        -fin: array of geometry (faces) of the mesh currently mapped to clip_space
        -fout: address to output the array of clipped geometry (faces) outwards
    */

    std::vector<v3F>* final_ndc_vertices = new std::vector<v3F>();
    std::vector<v3F> new_faces;

    for (auto& face : fin) {
        //we iterate through all the faces
        v4F v0 = (*clip_space)[(int)face.x - 1];
        v4F v1 = (*clip_space)[(int)face.y - 1];
        v4F v2 = (*clip_space)[(int)face.z - 1];

        //we clip acordingly
        std::vector<v4F> clipped = clipTriangle({ v0, v1, v2 });
        if (clipped.size() < 3) continue;

        for (int i = 1; i + 1 < clipped.size(); ++i) {
            v4F a = clipped[0];
            v4F b = clipped[i];
            v4F c = clipped[i + 1];

            //avoid division by 0
            a.w = (fabs(a.w) < 1e-6f) ? ((a.w >= 0) ? 1e-6f : -1e-6f) : a.w;
            b.w = (fabs(b.w) < 1e-6f) ? ((b.w >= 0) ? 1e-6f : -1e-6f) : b.w;
            c.w = (fabs(c.w) < 1e-6f) ? ((c.w >= 0) ? 1e-6f : -1e-6f) : c.w;

            //we pass the vertex by the NDC
            v3F na = { a.x / a.w, a.y / a.w, a.z / a.w };
            v3F nb = { b.x / b.w, b.y / b.w, b.z / b.w };
            v3F nc = { c.x / c.w, c.y / c.w, c.z / c.w };

            //checking degenerate vertex
            if (equals(na, nb) || equals(nb, nc) || equals(nc, na)) continue;

            //checking for near 0 data
            v3F ab = { nb.x - na.x, nb.y - na.y, nb.z - na.z };
            v3F ac = { nc.x - na.x, nc.y - na.y, nc.z - na.z };
            v3F cross_vec = cross(ab, ac);
            float area2 = sqrt(cross_vec.x * cross_vec.x + cross_vec.y * cross_vec.y + cross_vec.z * cross_vec.z);
            if (area2 < 1e-5f) continue;

            //we insert the new vertex
            int base_idx = (int)final_ndc_vertices->size();
            final_ndc_vertices->push_back(na);
            final_ndc_vertices->push_back(nb);
            final_ndc_vertices->push_back(nc);

            //we insert the new faces
            new_faces.emplace_back(v3F{
                (float)base_idx + 1,
                (float)base_idx + 2,
                (float)base_idx + 3
            });
        }
    }

    fout = new_faces;
    return final_ndc_vertices;
}


bool inside(const v4F& v, int plane) {
    switch (plane) {
        case 0: return v.x >= -v.w; // left
        case 1: return v.x <=  v.w; // right
        case 2: return v.y >= -v.w; // bottom
        case 3: return v.y <=  v.w; // top
        case 4: return v.z >= -v.w; // near
        case 5: return v.z <=  v.w; // far
    }
    return false;
}

v4F interpolate(const v4F& a, const v4F& b, int plane) {
    float num, den;

    switch (plane) {
        case 0: num = a.x + a.w; den = (a.x + a.w) - (b.x + b.w); break; // Left
        case 1: num = a.w - a.x; den = (a.w - a.x) - (b.w - b.x); break; // Right
        case 2: num = a.y + a.w; den = (a.y + a.w) - (b.y + b.w); break; // Bottom
        case 3: num = a.w - a.y; den = (a.w - a.y) - (b.w - b.y); break; // Top
        case 4: num = a.z + a.w; den = (a.z + a.w) - (b.z + b.w); break; // Near
        case 5: num = a.w - a.z; den = (a.w - a.z) - (b.w - b.z); break; // Far
        default: num = 0.0f; den = 1.0f;
    }

    if (fabs(den) < 1e-6f) {
        den = (den >= 0.0f) ? 1e-6f : -1e-6f;
    }

    float t = num / den;

    v4F out;
    out.x = a.x + t * (b.x - a.x);
    out.y = a.y + t * (b.y - a.y);
    out.z = a.z + t * (b.z - a.z);
    out.w = a.w + t * (b.w - a.w);

    return out;
}

std::vector<v4F> clipAgainstPlane(const std::vector<v4F>& poly, int plane) {
    std::vector<v4F> out;

    for (int i = 0; i < poly.size(); ++i) {
        const v4F& current = poly[i];
        const v4F& prev = poly[(i + poly.size() - 1) % poly.size()];
        bool curr_in = inside(current, plane);
        bool prev_in = inside(prev, plane);

        if (curr_in && prev_in) { 
            out.push_back(current);
        } else if (!curr_in && prev_in) {
            v4F inter = interpolate(prev, current, plane);
            out.push_back(inter);
        } else if (curr_in && !prev_in) {
            v4F inter = interpolate(current, prev, plane); 
            out.push_back(inter);
            out.push_back(current);
        }
    }

    return out;
}


std::vector<v4F> clipTriangle(std::vector<v4F> tri) {

    for (int plane = 0; plane < 6; ++plane) {
        tri = clipAgainstPlane(tri, plane);
        if (tri.empty()) {
            break; // completely clipped
        }
        if (tri.size() < 3) {
            tri.clear();
            break;
        }
    }

    return tri;
}

std::vector<v2F>* Transform::NDC_to_screen(std::vector<v3F>* in,int width,int height){
    std::vector<v2F>* out = new std::vector<v2F>();
    float rx = (float)width/2;
    float ry = (float)height/2;

    for(int i=0;i<in->size();i++){
        v2F into;
        into.x = ((*in)[i].x + 1) * rx;
        into.y = (1 - (*in)[i].y) * ry;
        into.x = std::clamp(into.x, 0.f, (float)width);
        into.y = std::clamp(into.y, 0.f, (float)height);
        out->push_back(into);
    }

    return out;
}

