#ifndef _TOOLS_H_
#define _TOOLS_H_

#include<cmath>
#include<cstdlib>
#include<limits>
#include<memory>
#include<functional>
#include<random>

#include"Vec3.h"
#include"ray.h"
#include"object.h"


const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.14159265535897932385;

// 包围盒 in tools.h
class bounding_box{
public:
    Vec3 p1, p2; // p1 为左下角，p2为右下角

public:
    bounding_box() : p1(Vec3()), p2(Vec3()){}
    bounding_box(const Vec3 &v1, const Vec3 &v2) : p1(v1), p2(v2){}
    bounding_box(const bounding_box &v) : p1(v.p1), p2(v.p2){}
    bool hit(const ray&r) const{
        auto res_dir = 1 / r.dir;
        auto t1 = (p1 - r.pos) * res_dir, t2 = (p2 - r.pos) * res_dir;
        auto k1 = Min(t1, t2), k2 = Max(t1, t2);
        auto s1 = k1.max(), s2 = k2.min();
        return s2 >= 0 && s2 >= s1;
    }                                                                                                                           
};

// 求包围盒的包围盒 两小求一大
bounding_box surrounding_box(const bounding_box& a, const bounding_box& b){
    Vec3 t1{std::min(a.p1.x, b.p1.x), std::min(a.p1.y, b.p1.y), std::min(a.p1.z, b.p1.z)};
    Vec3 t2{std::max(a.p2.x, b.p2.x), std::max(a.p2.y, b.p2.y), std::max(a.p2.z, b.p2.z)};
    return bounding_box{t1, t2};
}

bounding_box surrounding_box(const std::shared_ptr<bounding_box>& a, const std::shared_ptr<bounding_box>& b){
    return bounding_box{Min(a->p1, b->p1), Max(a->p2, b->p2)};
}


class texture{
public:
    virtual Vec3 tex_value(const Vec3 &vv, const double &u, const double &v) const = 0;
};

// 常量纹理值
class constant_texture : public texture{
public:
    Vec3 color;


public:
    constant_texture() : color(Vec3()){}
    constant_texture( const Vec3& v): color(v){}
    virtual Vec3 tex_value(const Vec3 &vv, const double &u, const double &v) const;
};

Vec3 constant_texture::tex_value(const Vec3 &vv, const double &u, const double &v) const{

    return color;
}

// 创建方格图案的纹理效果
class checker_texture : public texture{
public:
    std::shared_ptr<texture> odd, even;

public:
    checker_texture() : odd(nullptr), even(nullptr){}
    checker_texture(const std::shared_ptr<texture>& a, const std::shared_ptr<texture>& b) : odd(a), even(b) {}
    virtual Vec3 tex_value(const Vec3 &vv, const double &u, const double &v) const;
};

//角度转弧度
inline double degre_to_radians(const double& degre){
    return degre * pi / 180;
}

//随机数生成器
inline double random_double(){
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    static std::function<double()> rand_generator = std::bind(distribution, generator);
    return rand_generator();
}

//生成一个在 min max范围间的随机数
inline double random_double(const double& min,const double& max){
    return min + (max - min) * random_double();
}

inline int random_int(const double& min, const double& max){
    return (int)random_double(min, max);
}


inline double clamp(const double& x, const double& xmin, const double& xmax){
    if( x < xmin)
        return xmin;
    if( x > xmax )
        return xmax;
    return x;
}

void write_color(std::ostream &out, const Vec3 &v, const double& samples_per_pixel)
{
    auto scale = 1 / samples_per_pixel;
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * clamp(std::sqrt(v.x * scale), 0, 0.999)) << ' '
        << static_cast<int>(255.999 * clamp(std::sqrt(v.y * scale), 0, 0.999)) << ' '
        << static_cast<int>(255.999 * clamp(std::sqrt(v.z * scale), 0, 0.999)) << '\n';
    // gamma 矫正
}

// 生成一个随机向量
inline static Vec3 random(const double& min,const double& max){
    return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

// 在一个球体内生成一个单位向量 用于漫反射材质的反射方向，但是这个不准确
Vec3 random_in_unitsphere(){
    while(true){
        auto p = random(-1, 1);
        if( length(p) > 1 )
            continue;
        return p;
    }
}

// 随机生成一个向量 用于确定漫反射的方向，这个更准确
Vec3 random_in_vector(){
    auto a = random_double(0, 2 * pi);
    auto z = random_double(-1, 1);
    auto r = std::sqrt(1 - z * z);
    return Vec3(r * cos(a), r * sin(a), z);
}

Vec3 random_in_hemisphere(const Vec3& normal) {
    Vec3 in_unit_sphere = random_in_unitsphere();
    if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}
// 从一个单位小圆盘射出光线
Vec3 random_in_unit_disk() {
    while (true) {
        auto p = Vec3(random_double(-1,1), random_double(-1,1), 0);
        if ((length(p)) >= 1)
            continue;
        return p;
    }
}
//镜面反射
Vec3 reflect(const Vec3&v, const Vec3& n){
    return v - 2 * dot(v, n) * n; // 镜面反射
}
//折射
Vec3 refract(const Vec3 &r_in, const Vec3 &n, double n1_over_n2){
    auto cos_theta = dot(-r_in, n);
    Vec3 r_out_parallel = n1_over_n2 * (r_in + cos_theta * n);
    Vec3 r_out_perp = -std::sqrt(1 - length(r_out_parallel)) * n;
    return r_out_parallel + r_out_perp;
}

double schlick(const double& cosine,const double& ref_idx) {
    auto r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine),5);
}// 发生折射的概率 会随着入射角改变 这是一个近似等式

inline bool box_x_compare(const std::shared_ptr<object>& a, const std::shared_ptr<object>& b){
    auto k1 = a->get_aabb(), k2 = b->get_aabb();
    if( !k2 || !k2 ){
        std::cerr << "No bounding box can be found" << std::endl;
    } 
    return k1->p1.x < k2->p1.x;
}

inline bool box_y_compare(const std::shared_ptr<object>& a, const std::shared_ptr<object>& b){
    auto k1 = a->get_aabb(), k2 = b->get_aabb();
    if( !k2 || !k2 ){
        std::cerr << "No bounding box can be found" << std::endl;
    } 
    return k1->p1.y < k2->p1.y;
}

inline bool box_z_compare(const std::shared_ptr<object>& a, const std::shared_ptr<object>& b){
    auto k1 = a->get_aabb(), k2 = b->get_aabb();
    if( !k2 || !k2 ){
        std::cerr << "No bounding box can be found" << std::endl;
    } 
    return k1->p1.z < k2->p1.z;
}

Vec3 checker_texture::tex_value(const Vec3 &vv, const double &u, const double &v) const{
    auto sines = sin(10*vv.x)*sin(10*vv.y)*sin(10*vv.z);;
    if( sines < 0 )
        return odd->tex_value(vv, u, v);
    else
        return even->tex_value(vv, u, v);
}
#endif