#ifndef _RAY_H_
#define _RAY_H_

#include"Vec3.h"

class ray{
public:
    Vec3 pos, dir;
    double t; // 记录存在的时刻

public:
    ray() : pos{}, dir{}, t{} {}
    ray(const Vec3 &p, const Vec3 &d, const double& tt) : pos(p), dir(normalize(d)), t(tt){}
    Vec3 at(double t) const { return pos + dir * t; }
};


#endif