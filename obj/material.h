#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include"../tool/ray.h"
#include"../tool/Vec3.h"
#include"../tool/object.h"
class material{
public:
    virtual bool scatter(const ray &r_in, const hit_record &rec, Vec3 &attenuation, ray &scattered) const = 0;
};

//漫反射材料 发生漫反射， 随机方向反射
class lambertian : public material{
public:
    Vec3 albedo; // 反射率
public:
    lambertian() : albedo(Vec3()){}
    lambertian(const Vec3 &v) : albedo(v){}

    virtual bool scatter(const ray &r_in, const hit_record &rec, Vec3 &attenuation, ray &scattered) const{
        Vec3 scatter_dir = rec.normal + random_in_vector();
        scattered = ray(rec.hitpoint, scatter_dir);
        attenuation = albedo;
        return true;
    }
};

// 金属材质 发生镜面反射
class metal : public material{
public:
    Vec3 albedo;
    double fuzz;

public:
    metal() : albedo(Vec3()){}
    metal(const Vec3 &v, const double& f) : albedo(v), fuzz(f){}
    virtual bool scatter(const ray& r_in,const hit_record& rec, Vec3& attenuation, ray& scattered) const{
        Vec3 scatter_dir = reflect(r_in.dir, rec.normal);
        scattered = ray(rec.hitpoint, scatter_dir + fuzz *random_in_hemisphere(rec.normal));
        attenuation = albedo;
        return dot(scattered.dir, rec.normal) > 0;
    }
};

// 绝缘体材料 发生折射或者是镜面反射
class dielectric : public material{
public:
    double ref_idx;

public:
    dielectric() : ref_idx(0){}
    dielectric(const double &v) : ref_idx(v){}
    virtual bool scatter(const ray& r_in,const hit_record& rec, Vec3& attenuation, ray& scattered) const{
        attenuation = Vec3{1, 1, 1};
        double n1_n2 = rec.face_front ? 1.0 / ref_idx : ref_idx;

        double cos_theta = dot(-r_in.dir, rec.normal);
        double sin_theta = std::sqrt(1 - cos_theta * cos_theta);
        double refract_prob = schlick(cos_theta, n1_n2); //发生折射的概率 与随机数比较 or 不满足折射条件 则发生镜面反射
        if( n1_n2 * sin_theta > 1  || random_double()  < refract_prob ) {
            Vec3 reflected = reflect(r_in.dir, rec.normal);
            scattered = ray(rec.hitpoint, reflected);
            return true;
        }

        Vec3 refracted = refract(r_in.dir, rec.normal, n1_n2);
        scattered = ray(rec.hitpoint, refracted);
        return true;
    }
};

#endif