#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "tools.h"
#include"Vec3.h"
#include"ray.h"

class camera{
public:
    Vec3 origin, vertical, horizontal, lower_left_corner;
    Vec3 u, v, w; // 三个轴，x y z 轴
    double lens_radius; //光圈大小

public:
//up 指向 y轴正半轴 lookat 看向z轴负半轴
    camera(const Vec3& pos, const Vec3& lookat, const Vec3& up, const float& fov, const float& aspect,
        const double& aperture,const double& foucus_dist){ // focus_dist 为焦距 aperture 为光圈大小
        origin = pos; //设置原点位置
        lens_radius = aperture / 2;
        double theta = degre_to_radians(fov / 2);
        double half_height = tan(theta);
        double half_width = half_height * aspect;

        w = normalize(pos - lookat); // 指向z的正半轴，因为摄像机时看向z的负半轴

        u = normalize(cross(up, w));
        v = -normalize(cross(u, w)); // 这才是正确的y轴方向

        lower_left_corner = origin - foucus_dist * half_width * u - half_height * foucus_dist * v - foucus_dist * w; // 左下角的点
        horizontal = 2 * foucus_dist * half_width * u;
        vertical = 2 * foucus_dist * half_height * v;
    }

    ray get_ray(double u, double v){
        Vec3 rd = lens_radius * random_in_unit_disk();
        Vec3 offset = camera::u * rd.x + camera::v * rd.y;

        return ray(origin+offset, lower_left_corner + u*horizontal + v*vertical - origin-offset);
    }
};

#endif