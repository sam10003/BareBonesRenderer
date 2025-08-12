
#pragma once

/**
 * various definitions of the vector data structures used in this project
 * 
 *  v2F - R2 vector x, y
 *    (we only use it as a datatype in the rasterizer so no special functions)
 *  v3F - R3 vector x, y, z
 *  v4F - R4 vector x, y, z, w
 *    (both v3F and v4F have arithmetic and algebraic functions for their applications)
 *  M4x4 - matrix 4x4 
 */


// -- v2F --
struct v2F {
    float x, y;

    v2F() : x(0), y(0) {}              
    v2F(float a, float b) : x(a), y(b) {}  

    void print() const;  
};


// -- v3F -- 
struct v3F {
    float x, y, z;

    v3F() : x(0), y(0), z(0) {}
    v3F(float a, float b, float c) : x(a), y(b), z(c) {}

    void print() const;
};

//arithmetic functions
v3F operator+(const v3F& a, const v3F& b);
v3F operator-(const v3F& a, const v3F& b);
v3F operator*(const v3F& a, float s);
v3F operator*(float s, const v3F& a);
v3F operator/(const v3F& a, float s);
bool equals(const v3F& a, const v3F& b, float epsilon = 1e-6f);

//linear algebra functions
float length(const v3F& v); //will return the numerical length of said vector
v3F normalize(const v3F& v); //will return the basis of the inputed vector
v3F cross(const v3F& a, const v3F& b); //will return the cross product
float dot(const v3F& a, const v3F& b); //will return the dot product


// -- v4F -- 
struct v4F {
    float x, y, z, w;

    v4F() : x(0), y(0), z(0), w(0) {}
    v4F(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {}

    void print() const;
};

//arithmetic functions
v4F operator+(const v4F& a, const v4F& b);
v4F operator-(const v4F& a, const v4F& b);
v4F operator*(const v4F& a, float s);
v4F operator*(float s, const v4F& a);
v4F operator/(const v4F& a, float s);


// -- M4x4 -- 
struct M4x4 {
    float m[4][4];

    M4x4() {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                m[i][j] = 0.0f;
            }
        }
    }
};

void printMatrix(const M4x4& mat);
M4x4 operator*(const M4x4& a, const M4x4& b); //matrix multiplication

//linear transformation
v4F mat4_multiply_vec4(const M4x4& m, const v4F& v);