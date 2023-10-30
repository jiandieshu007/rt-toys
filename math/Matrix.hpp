#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_

#include "Vector.hpp"

#include<cstring>


struct Mat4f{
    float data[4][4];
    inline Mat4f(){
        memset(data, 0, sizeof(data));
    }
    explicit Mat4f(float mat[4][4]) {
        memcpy(data, mat, 16 * sizeof(float));
    }
    Mat4f(float t00, float t01, float t02, float t03,
              float t10, float t11, float t12, float t13,
              float t20, float t21, float t22, float t23,
              float t30, float t31, float t32, float t33) {
        data[0][0] = t00;
        data[0][1] = t01;
        data[0][2] = t02;
        data[0][3] = t03;
        data[1][0] = t10;
        data[1][1] = t11;
        data[1][2] = t12;
        data[1][3] = t13;
        data[2][0] = t20;
        data[2][1] = t21;
        data[2][2] = t22;
        data[2][3] = t23;
        data[3][0] = t30;
        data[3][1] = t31;
        data[3][2] = t32;
        data[3][3] = t33;
    }
    inline Mat4f(const Mat4f& mat){
        for (size_t i = 0; i < 4; ++i){
            for (size_t j = 0; j < 4; ++j){
                data[i][j] = mat.data[i][j];
            }
        }
    };
    /*
    inline float* operator[](const size_t& i) const{
        assert(i < 4);
        return data[i];
    };
    inline float* operator[](size_t&& i) const{
        assert(i < 4);
        return data[i];
    };
    inline float* operator[](const size_t& i){
        assert(i < 4);
        return data[i];
    }
    inline  float* operator[](size_t&& i){
        assert(i < 4);
        return data[i];
    }*/
    float* operator[](size_t index) {
        assert(index < 4);
        return data[index];
    }

    const float* operator[](size_t index) const {
        assert(index < 4);
        return data[index];
    }

};

Mat4f operator*(const Mat4f& a,const float& b ){
    Mat4f ret(a);
    for (size_t i = 0; i < 4; ++i){
        for (size_t j = 0; j < 4; ++j){
            ret[i][j] *= b;
        }
    }
    return ret;
}
Mat4f operator+(const Mat4f& a,const float& b ){
    Mat4f ret(a);
    for (size_t i = 0; i < 4; ++i){
        for (size_t j = 0; j < 4; ++j){
            ret[i][j] += b;
        }
    }
    return ret;
}
Mat4f operator-(const Mat4f& a,const float& b ){
    Mat4f ret(a);
    for (size_t i = 0; i < 4; ++i){
        for (size_t j = 0; j < 4; ++j){
            ret[i][j] -= b;
        }
    }
    return ret;
}
Mat4f operator/(const Mat4f& a,const float& b ){
    Mat4f ret(a);
    for (size_t i = 0; i < 4; ++i){
        for (size_t j = 0; j < 4; ++j){
            ret[i][j] /= b;
        }
    }
    return ret;
}

Mat4f operator*(const Mat4f&a, const Mat4f&b ){
    Mat4f ret;
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            for (size_t k = 0; k < 4; ++k) {
                ret[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return ret;
}

inline Mat4f Identity(){
    Mat4f ret;
    for (size_t i = 0; i < 4; ++i){
        ret[i][i] = 1;
    }
    return ret;
}
inline Mat4f Transpose(const Mat4f& mat) {
    return {
            mat[0][0], mat[1][0], mat[2][0], mat[3][0],
            mat[0][1], mat[1][1], mat[2][1], mat[3][1],
            mat[0][2], mat[1][2], mat[2][2], mat[3][2],
            mat[0][3], mat[1][3], mat[2][3], mat[3][3]
    };
}

//伴随矩阵
inline Mat4f Adjoint(const Mat4f& mat) {
    Mat4f adjoint;
    //4x4遍历
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            //3x3余子式
            float std::minor3x3[3][3];
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    if (i != row && j != col) {
                        std::minor3x3[i > row ? i - 1 : i][j > col ? j - 1 : j] = mat[i][j];
                    }
                }
            }
            //余子式行列式
            for (int i = 0; i < 3; ++i) {
                if (std::minor3x3[0][i] != 0.0f) {
                    float det = std::minor3x3[1][(i + 1) % 3] * std::minor3x3[2][(i + 2) % 3] -
                                std::minor3x3[1][(i + 2) % 3] * std::minor3x3[2][(i + 1) % 3];
                    adjoint[col][row] += std::minor3x3[0][i] * det;
                }
            }
            adjoint[col][row] *= (row + col) % 2 == 0 ? 1.0f : -1.0f;
        }
    }
    return adjoint;
}

//逆矩阵
inline Mat4f Inverse(const Mat4f& mat) {
    Mat4f adjoint = Adjoint(mat);
    float det = 0.0f;
    for (int i = 0; i < 4; ++i) {
        det += mat[0][i] * adjoint[i][0];
    }
    return adjoint / det;
}

#endif