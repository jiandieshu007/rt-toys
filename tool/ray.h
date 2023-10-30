#ifndef _RAY_H_
#define _RAY_H_

#include"Vec3.h"

class ray{
public:
    Vec3 pos, dir;

public:
    ray() : pos{}, dir{} {}
    ray(const Vec3 &p, const Vec3 &d) : pos(p), dir(normalize(d)){}
    Vec3 position() const { return pos; }
    Vec3 direction() const { return dir; }
    Vec3 at(double t) const { return pos + dir * t; }
};


#endif