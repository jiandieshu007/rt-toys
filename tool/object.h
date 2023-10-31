#ifndef _OBJECT_H_
#define _OBJECT_H_

#include"ray.h"
#include"tools.h"

class material;
class bounding_box;

struct hit_record{
    Vec3 hitpoint;
    Vec3 normal;
    double t; //击中的时间
    double u, v;
    std::shared_ptr<material> material_ptr; //存储了什么材质的材料 决定了光线打在表面之后如何传播
    bool face_front; // true 表示从外部射入的光线 false反之

    inline void set_face_front(const Vec3& ray_dir, const Vec3& pre_normal){
        face_front = dot(ray_dir, pre_normal) > 0;
        normal = face_front > 0 ? -pre_normal : pre_normal;
    }
};

class object{
public:
    virtual bool hit(const ray &r, const double& tmin, const double& tmax, hit_record &rec) const = 0;
    virtual std::shared_ptr<bounding_box> get_aabb() const = 0;
};

#endif