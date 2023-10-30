#ifndef _Vec3_H_
#define _Vec3_H_


#include<iostream>
#include<cmath>
#include "tools.h"

class Vec3{
public:
    union{
        double data[3];
        struct{
            double x, y, z;
        };
    };
    Vec3() : data{0, 0, 0} {}
    Vec3(const double& xx,const double& yy,const double& zz) : x(xx), y(yy), z(zz){}
    Vec3(const Vec3& v): x(v.x),y(v.y),z(v.z){};

        double
        operator[](int i) const
    {
        return data[i];
    }
    double &operator[](int i) { return data[i]; }
    Vec3 operator-() const { return Vec3{-x, -y, -z}; }

    Vec3& operator+=(const Vec3& a){
        x += a.x;
        y += a.y;
        z += a.z;
        return *this;
    }
    Vec3 operator*=(const Vec3&a){
        x *= a.x;
        y *= a.y;
        z *= a.z;
        return *this;
    }
};

inline double length(const Vec3& v){
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}


inline double dot(const Vec3 &u, const Vec3 &v) {
    return u.x * v.x
         + u.y * v.y
         + u.z * v.z;
}

inline Vec3 cross(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.y * v.z - u.z * v.y,
                u.z * v.x - u.x * v.z,
                u.x * v.y - u.y * v.x);
}




inline std::ostream& operator<<(std::ostream &out, const Vec3 &v) {
    return out << v.x << ' ' << v.y << ' ' << v.z;
}


inline Vec3 operator+(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline Vec3 operator-(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

inline Vec3 operator*(const Vec3 &u, const Vec3 &v) {
    return Vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}

inline Vec3 operator*(double t, const Vec3 &v) {
    return Vec3(t*v.x, t*v.y, t*v.z);
}

inline Vec3 operator*(const Vec3 &v, double t) {
    return t * v;
}

inline Vec3 operator/(Vec3 v, double t) {
    return (1/t) * v;
}

inline Vec3 normalize(Vec3 v) {
    return v / length(v);
}

inline Vec3 lerp(const Vec3&a, const Vec3&b, double t){
    return (1 - t) * a + t * b;
}

#endif