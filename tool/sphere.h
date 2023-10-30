#ifndef _SPHERE_H_
#define _SPHERE_H_

#include"object.h"
#include"Vec3.h"

class sphere: public object{
public:
    Vec3 center;
    double radius;
    std::shared_ptr<material> material_ptr;

public:
    sphere() = default;
    sphere(const Vec3 &v, const double &t,std::shared_ptr<material> m) : center(v), radius(t),material_ptr(m){}
    virtual bool hit(const ray &r,const double& tmin,const double& tmax, hit_record &rec) const;
    virtual std::shared_ptr<bounding_box> get_aabb() const;
};

bool sphere::hit(const ray&r, const double& tmin,const double& tmax, hit_record &rec) const{
    Vec3 pc = r.pos - center;
    double a = dot(r.dir, r.dir), b = 2 * dot(r.dir, pc), c = dot(pc, pc) - radius * radius;
    double cc = b * b - 4 * a * c;
    if( cc >0 ){
        double ccc = std::sqrt(cc);
        double t1 = (-b - ccc) / 2 / a, t2 = (-b + ccc) / 2 / a;
        if( t1>tmin && t1 < tmax){
            rec.t = t1;
            rec.hitpoint = r.at(t1);
            Vec3 normal = normalize(rec.hitpoint - center);
            rec.set_face_front(r.dir, normal);
            rec.material_ptr = material_ptr;
            return true;
        }
        if( t2 > tmin && t2 < tmax){
            rec.t = t2;
            rec.hitpoint = r.at(t2);
            auto normal = normalize(rec.hitpoint - center);
            rec.set_face_front(r.dir, normal);
            rec.material_ptr = material_ptr;
            return true;
        }
    }
    return false;
}   

std::shared_ptr<bounding_box> sphere::get_aabb() const{
    Vec3 k{radius, radius, radius};
    return std::make_shared<bounding_box>(center - k, center + k);
}
#endif