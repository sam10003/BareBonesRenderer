
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "core/mesh.hpp"

#include "utils/log.hpp"

//construction & deconstruction
Mesh::Mesh(){
    v = new std::vector<v3F>();
    v4 = new std::vector<v4F>();
    vn = new std::vector<v3F>();
    vt = new std::vector<v3F>();
    f = new std::vector<v3F>();

    transform_v4 = new std::vector<v4F>();
    ndc_v = new std::vector<v3F>();
    ndc_f = new std::vector<v3F>();
    screen = new std::vector<v2F>();
}

Mesh::~Mesh(){
    delete v;
    delete v4;
    delete vn;
    delete vt;
    delete f;

    delete transform_v4;
    delete ndc_v;
    delete ndc_f;
    delete screen;
}

//getters and setters
std::vector<v3F>* Mesh::get_v(){
    return v;
}
std::vector<v4F>* Mesh::get_v4(){
    return v4;
}
std::vector<v3F>* Mesh::get_vn(){
    return vn;
}
std::vector<v3F>* Mesh::get_vt(){
    return vt;
}
std::vector<v3F>* Mesh::get_f(){
    return f;
}
std::vector<v4F>* Mesh::get_transform_v4(){
    return transform_v4;
}
std::vector<v3F>* Mesh::get_ndc_v(){
    return ndc_v;
}
std::vector<v3F>* Mesh::get_ndc_f(){
    return ndc_f;
}
std::vector<v2F>* Mesh::get_screen(){
    return screen;
}

void Mesh::set_v(std::vector<v3F>* new_v){
    if(new_v == nullptr){return;}
    *v = *new_v;
}
void Mesh::set_v4(std::vector<v4F>* new_v4){
    if(new_v4 == nullptr){return;}
    *v4 = *new_v4;
}
void Mesh::set_vn(std::vector<v3F>* new_vn){
    if(new_vn == nullptr){return;}
    *vn = *new_vn;
}
void Mesh::set_vt(std::vector<v3F>* new_vt){
    if(new_vt == nullptr){return;}
    *vt = *new_vt;
}
void Mesh::set_f(std::vector<v3F>* new_f){
    if(new_f == nullptr){return;}
    *f = *new_f;
}
void Mesh::set_transform_v4(std::vector<v4F>* new_transform_v4){
    if(new_transform_v4 == nullptr){return;}
    *transform_v4 = *new_transform_v4;
}
void Mesh::set_ndc_v(std::vector<v3F>* new_ndc_v){
    if(new_ndc_v== nullptr){return;}
    *ndc_v = *new_ndc_v;
}
void Mesh::set_ndc_f(std::vector<v3F>* new_ndc_f){
    if(new_ndc_f == nullptr){return;}
    *ndc_f = *new_ndc_f;
}
void Mesh::set_screen(std::vector<v2F>* new_screen){
    if(new_screen == nullptr){return;}
    *screen = *new_screen;
}



void Mesh::open_obj(info_dump *log, std::fstream& file){
    //temporal allocated vectors
    std::vector<float*> v_p;
    std::vector<float*> vt_p;
    std::vector<float*> vn_p;
    std::vector<std::vector<int>> f_p;


    parse_obj(file,log,v_p,vn_p,vt_p,f_p); //parsed
    raw_to_struct(v_p,vn_p,vt_p,f_p); //to class
    rm_obj(v_p,vn_p,vt_p,f_p); //removed
}

void Mesh::parse_obj(std::fstream& file, info_dump *log, std::vector<float*>& v_p,
                     std::vector<float*>& vn_p, std::vector<float*>& vt_p, std::vector<std::vector<int>>& f_p)
{
    //this function parses .obj files
    //inserts them in their corresponding arrays
    std::string line;
    int line_c = 0;

    while (getline(file, line)) {
        const char *c = line.c_str();
        if(c[0] == 'v'){
            if(c[1] == 't'){
                float *ft = parse_vector_t(line);
                if(ft != nullptr){
                    vt_p.push_back(ft);
                }
            } else if (c[1] == 'n'){
                float *ft = parse_vector_n(line);
                if(ft != nullptr){
                    vn_p.push_back(ft);
                }
            } else {
                float *ft = parse_vector(line);
                if(ft != nullptr){
                    v_p.push_back(ft);
                }
            }
        } else if(c[0] == 'f'){
            std::vector<int> ft = parse_f(line);
            f_p.push_back(ft);
        }
        line_c++;
        if (file.eof()) break;
    }

    log->v_n = v_p.size();
    log->vt_n = vt_p.size();
    log->vn_n = vn_p.size();
    log->f_n = f_p.size();
    return;
}

float* Mesh::parse_vector(const std::string& l) 
{
    //vector converts
    // in -> [v]' '[float]' '[float]' '[float]
    // out <- float[3]

    std::stringstream ss(l);
    std::string w;
    std::vector<std::string> w_v;

    while (std::getline(ss, w, ' ')) {  
        w_v.push_back(w);
    }

    //length check
    if (w_v.size() != 4) {
        std::cerr << ".[parsing_v] anomalous (v) size found: " << w_v.size() << "\n";
        return nullptr;
    }

    if(w_v[0] != "v"){
        std::cerr << ".[parsing_v] expected (v) got: " << w_v[0] << "\n";
        return nullptr;
    }

    //actual parsing
    float* vector_3 = new float[3];
    for(int i = 0; i < 3; i++) {
        try {
            vector_3[i] = std::stof(w_v[i + 1]);  
        } catch (const std::exception& e) {  
            std::cerr << ".[parsing_v] failed stof: " << e.what() << "\n";
            delete[] vector_3;
            return nullptr;
        }
    }
    
    return vector_3; 
}
float* Mesh::parse_vector_t(const std::string& l)
{
    //vector converts
    // in -> [vt]' '[float]' '[float]'
    // out <- float[2]

    std::stringstream ss(l);
    std::string w;
    std::vector<std::string> w_v;

    while (std::getline(ss, w, ' ')) {  
        w_v.push_back(w);
    }

    //length check
    if (w_v.size() != 3) {
        std::cerr << ".[parsing_v] anomalous (vt) size found: " << w_v.size() << "\n";
        return nullptr;
    }

    if(w_v[0] != "vt"){
        std::cerr << ".[parsing_v] expected (vt) got: " << w_v[0] << "\n";
        return nullptr;
    }

    //actual parsing
    float* vector_2 = new float[2];
    for(int i = 0; i < 2; i++) {
        try {
            vector_2[i] = std::stof(w_v[i + 1]);  
        } catch (const std::exception& e) {  
            std::cerr << ".[parsing_v] failed stof: " << e.what() << "\n";
            delete[] vector_2;
            return nullptr;
        }
    }
    
    return vector_2; 
}
float* Mesh::parse_vector_n(const std::string& l) 
{
    //vector converts
    // in -> [vn]' '[float]' '[float]' '[float]
    // out <- float[3]

    std::stringstream ss(l);
    std::string w;
    std::vector<std::string> w_v;

    while (std::getline(ss, w, ' ')) {  
        w_v.push_back(w);
    }

    //length check
    if (w_v.size() != 4) {
        std::cerr << ".[parsing_v] anomalous (vn) size found: " << w_v.size() << "\n";
        return nullptr;
    }

    if(w_v[0] != "vn"){
        std::cerr << ".[parsing_v] expected (vn) got: " << w_v[0] << "\n";
        return nullptr;
    }

    //actual parsing
    float* vector_3 = new float[3];
    for(int i = 0; i < 3; i++) {
        try {
            vector_3[i] = std::stof(w_v[i + 1]);  
        } catch (const std::exception& e) {  
            std::cerr << ".[parsing_v] failed stof: " << e.what() << "\n";
            delete[] vector_3;
            return nullptr;
        }
    }
    
    return vector_3; 
}
std::vector<int> Mesh::parse_f(const std::string& l)
{
    //individual face parser
    // in -> [any chain of ints]
    // out <- int[said number of ints]

    std::istringstream ss(l);
    std::string token;
    bool first = true;
    std::vector<int> faces;

    while (ss >> token) {
        if (first) {
            if (token != "f")
                throw std::runtime_error(".[parsing_v] expected (f) got: " + token);
            first = false;
        } else {
            try {
                faces.push_back(std::stoi(token));
            } catch (const std::exception& e) {
                throw std::runtime_error(".[parsing_v] failed to parse int: " + std::string(e.what()));
            }
        }
    }
    return faces;
}

void Mesh::rm_obj(std::vector<float*>& v_p, std::vector<float*>& vn_p, std::vector<float*>& vt_p, std::vector<std::vector<int>>& f_p)
{
    for (auto ptr : v_p) {
        delete[] ptr;
    }
    for (auto ptr : vn_p) {
        delete[] ptr;
    }
    for (auto ptr : vt_p) {
        delete[] ptr;
    }

    v_p.clear();
    vn_p.clear();
    vt_p.clear();
    f_p.clear();
}

void Mesh::raw_to_struct(std::vector<float*>& v_p, std::vector<float*>& vn_p, std::vector<float*>& vt_p, std::vector<std::vector<int>>& f_p)
{
    std::vector<v3F> triangles;
    //before introducting this to the mesh we triangulate through the fan method
    for (size_t j = 0; j < f_p.size(); ++j) {
        const std::vector<int>& faceIndices = f_p[j];

        for (size_t i = 1; i + 1 < faceIndices.size(); ++i) {
            float v0 = (float)faceIndices[0];
            float v1 = (float)faceIndices[i];
            float v2 = (float)faceIndices[i + 1];
            v3F into;
            into.x = v0;
            into.y = v1;
            into.z = v2;
            triangles.push_back(into);
        }
    }

    v3F into;
    for(int i=0;i<v_p.size();i++){
        into.x = v_p[i][0];
        into.y = v_p[i][1];
        into.z = v_p[i][2];
        v->push_back(into);
    }

    for(int i=0;i<vn_p.size();i++){
        into.x = vn_p[i][0];
        into.y = vn_p[i][1];
        into.z = vn_p[i][2];
        vn->push_back(into);
    }

    for(int i=0;i<vt_p.size();i++){
        into.x = vt_p[i][0];
        into.y = vt_p[i][1];
        into.z = vt_p[i][2];
        vt->push_back(into);
    }

    for (const auto& tri : triangles) {
        f->push_back(tri);
    }

}

void Mesh::to_v4(){
    if(v == nullptr || v4 == nullptr){return;}
    if(v->size() == 0 || v4->size() != 0){return;}

    v4F into;
    for(int i=0;i<v->size();i++){
        into.x = (*v)[i].x;
        into.y = (*v)[i].y;
        into.z = (*v)[i].z;
        into.w = 1.00;
        v4->push_back(into);
    }
}

void Mesh::clean(info_dump *log){
    transform_v4->clear();
    ndc_v->clear();
    ndc_f->clear();
    screen->clear();
}