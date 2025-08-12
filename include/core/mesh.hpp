
#pragma once

/**
 * this class represents the data that the represented 3D object holds
 * it also has a .obj wavefront parser
 * */ 

#include <vector>

#include "utils/data_f.hpp"
#include "utils/log.hpp"


class Mesh {
    //baseline mesh data
    std::vector<v3F> *v;
    std::vector<v3F> *vn;
    std::vector<v3F> *vt;
    std::vector<v3F> *f;
    

    //pipeline staged data
    std::vector<v4F> *v4; //v4 format (normally computed before pipeline)
    std::vector<v4F> *transform_v4; //post view + perspective transform
    std::vector<v3F> *ndc_v; //post NDC + clipping
    std::vector<v3F> *ndc_f; //post NDC + clipping
    std::vector<v2F> *screen; //screen coords

    private:
        //parsing function
        void parse_obj(std::fstream& file, //.obj file
                       info_dump *log, //log
                       std::vector<float*> &v_p, std::vector<float*> &vn_p, std::vector<float*> &vt_p, std::vector<std::vector<int>> &f_p); // mesh data

        //auxiliary parsing functions
        float* parse_vector(const std::string& l); //for v case
        float* parse_vector_t(const std::string& l); //for vt case
        float* parse_vector_n(const std::string& l); //for vn case
        std::vector<int> parse_f(const std::string& l); //for f case
        
        //cleans the temporary allocated vectors
        void rm_obj(std::vector<float*>& v_p, std::vector<float*>& vn_p, std::vector<float*>& vt_p, std::vector<std::vector<int>>& f_p);

        //passes the temporary vectors to the actual class
        void raw_to_struct(std::vector<float*>& v_p, std::vector<float*>& vn_p, std::vector<float*>& vt_p, std::vector<std::vector<int>>& f_p);

    public:
        Mesh();
        ~Mesh();

        //setters & getters
        std::vector<v3F> *get_v();
        std::vector<v4F> *get_v4();
        std::vector<v3F> *get_vn();
        std::vector<v3F> *get_vt();
        std::vector<v3F> *get_f();
        std::vector<v4F> *get_transform_v4();
        std::vector<v3F> *get_ndc_v();
        std::vector<v3F> *get_ndc_f();
        std::vector<v2F> *get_screen();

        void set_v(std::vector<v3F> *new_v);
        void set_v4(std::vector<v4F> *new_v4);
        void set_vn(std::vector<v3F> *new_vn);
        void set_vt(std::vector<v3F> *new_vt);
        void set_f(std::vector<v3F> *new_f);
        void set_transform_v4(std::vector<v4F>* new_transform_v4);
        void set_ndc_v(std::vector<v3F>* new_ndc_v);
        void set_ndc_f(std::vector<v3F>* new_ndc_f);
        void set_screen(std::vector<v2F>* new_screen);

        //open the .obj file and build the mesh
        void open_obj(info_dump *log, std::fstream& file); 

        //initializes the v4 array (ease computing)
        void to_v4(); 

        //cleans the pipeline parts of the mesh (except v4)
        void clean(info_dump *log);
};