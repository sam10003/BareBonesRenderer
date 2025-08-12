
#include <iostream>
#include <cmath>

#include "utils/data_f.hpp"


// -- v2F -- 
void v2F::print() const {
    std::cout << "(" << x << "," << y << ")\n";
}


// -- v3F -- 
void v3F::print() const{
    std::cout << "(" << x << "," << y << "," << z << ")\n";
}

// arithmetic functions
v3F operator+(const v3F& a, const v3F& b) {
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}
v3F operator-(const v3F& a, const v3F& b) {
    return { a.x - b.x, a.y - b.y, a.z - b.z };
}
v3F operator*(const v3F& a, float s) {
    return { a.x * s, a.y * s, a.z * s };
}
v3F operator*(float s, const v3F& a) {
    return a * s;
}
v3F operator/(const v3F& a, float s) {
    return { a.x / s, a.y / s, a.z / s };
}
bool equals(const v3F& a, const v3F& b, float epsilon) {
    return (fabs(a.x - b.x) < epsilon) &&
           (fabs(a.y - b.y) < epsilon) &&
           (fabs(a.z - b.z) < epsilon);
}

// linear algebra functions
float length(const v3F& v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
v3F normalize(const v3F& v) {
    float len = length(v);
    if (len < 1e-8f) return {0, 0, 0}; 
    return { v.x / len, v.y / len, v.z / len };
}
v3F cross(const v3F& a, const v3F& b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}
float dot(const v3F& a, const v3F& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}


// -- v4F -- 
void v4F::print() const{
    std::cout << "(" << x << "," << y << "," << z << "," << w << ")\n";
}

//arithmetic functions
v4F operator+(const v4F& a, const v4F& b) {
    return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}
v4F operator-(const v4F& a, const v4F& b) {
    return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
}
v4F operator*(const v4F& a, float s) {
    return { a.x * s, a.y * s, a.z * s, a.w * s };
}
v4F operator*(float s, const v4F& a) {
    return a * s;
}
v4F operator/(const v4F& a, float s) {
    return { a.x / s, a.y / s, a.z / s, a.w / s };
}


// -- M4x4 -- 
void printMatrix(const M4x4& mat){
    for (int i = 0; i < 4; ++i) {
        std::cout << "[ ";
        for (int j = 0; j < 4; ++j) {
            std::cout << mat.m[i][j];
            if (j < 3) std::cout << ", ";
        }
        std::cout << " ]" << std::endl;
    }
}

//matrix multiplication
M4x4 operator*(const M4x4& a, const M4x4& b) {
    M4x4 result = {}; // zero initialize

    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            float sum = 0.0f;
            for (int k = 0; k < 4; ++k) {
                sum += a.m[row][k] * b.m[k][col];
            }
            result.m[row][col] = sum;
        }
    }

    return result;
}

//linear transformation
v4F mat4_multiply_vec4(const M4x4& m, const v4F& v) {
    return {
        m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w,
        m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w,
        m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w,
        m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w
    };
}