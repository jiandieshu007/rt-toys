#include <iostream>



#include"tool/camera.h"
#include"tool/hit_list.h"
#include"tool/sphere.h"
#include"tool/tools.h"
#include"obj/material.h"
#include"acceleration/bvh.h"


Vec3 ray_color(const ray&r, const hit_list& world, int depth){
    if( depth == 0 ){
        return Vec3();
    }
    hit_record rec;
    if(world.hit(r,0,infinity,rec)){
        ray scattered;
        Vec3 attenuation;
        if(rec.material_ptr->scatter(r,rec,attenuation,scattered) ){
            return attenuation * ray_color(scattered, world, depth-1);
        }
        return {0, 0, 0};
    }
    double t = 0.5*(r.dir.y + 1);
    return (1 - t) * Vec3{1, 1, 1} + t * Vec3{0.5, 0.7, 1.0};
}

const Vec3 pos(13, 2, 3), lookat(0, 0, 0), up(0, 1, 0);
double dist_to_focus = 10, aperture = 0.0;

hit_list random_scene() {
    hit_list world;

    world.add(std::make_shared<sphere>(
        Vec3(0,-1000,0), 1000, std::make_shared<lambertian>(Vec3(0.5, 0.5, 0.5))));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Vec3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());
            if (length(center - Vec3(4, 0.2, 0)) > 0.9) {
                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = random_in_vector() * random_in_vector();
                    world.add(
                        std::make_shared<sphere>(center, 0.2, std::make_shared<lambertian>(albedo)));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = random(.5, 1);
                    auto fuzz = random_double(0, .5);
                    world.add(
                        std::make_shared<sphere>(center, 0.2, std::make_shared<metal>(albedo, fuzz)));
                } else {
                    // glass
                    world.add(std::make_shared<sphere>(center, 0.2, std::make_shared<dielectric>(1.5)));
                }
            }
        }
    }

    world.add(std::make_shared<sphere>(Vec3(0, 1, 0), 1.0, std::make_shared<dielectric>(1.5)));

    world.add(
        std::make_shared<sphere>(Vec3(-4, 1, 0), 1.0, std::make_shared<lambertian>(Vec3(0.4, 0.2, 0.1))));

    world.add(
        std::make_shared<sphere>(Vec3(4, 1, 0), 1.0, std::make_shared<metal>(Vec3(0.7, 0.6, 0.5), 0.0)));

    return static_cast<hit_list>(std::make_shared<bvh>(world.objects, 0, world.objects.size()));
}

hit_list two_spheres() {
    hit_list objects;

    auto checker = std::make_shared<checker_texture>(
        std::make_shared<constant_texture>(Vec3(0.2, 0.3, 0.1)),
        std::make_shared<constant_texture>(Vec3(0.9, 0.9, 0.9))
    );

    objects.add(std::make_shared<sphere>(Vec3(0,-10, 0), 10, std::make_shared<texture_lambertian>(checker)));
    objects.add(std::make_shared<sphere>(Vec3(0, 10, 0), 10, std::make_shared<texture_lambertian>(checker)));

    return static_cast<hit_list>(std::make_shared<bvh>(objects.objects, 0, objects.objects.size()));
}

int main() {
    const int image_width = 200, image_height = 100, sample_per_pixel = 1000;
    const double aspect = image_width / image_height;

    std::cout << "P3\n"
              << image_width <<
        " " << image_height << "\n255\n";

    camera cam(pos, lookat, up, 20, aspect, aperture, dist_to_focus, 0, 1);
    auto world = two_spheres();
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            Vec3 color(0, 0, 0);
            for (int k = 0; k < sample_per_pixel; ++k){
                auto u = (i+random_double()) / image_width;
                auto v = (j + random_double()) / image_height;
                ray r = cam.get_ray(u, v);
                color += ray_color(r, world, 100);
            }

            write_color(std::cout,color,sample_per_pixel);
        }
    }
    std::cerr << "\nDone\n";
    return 0;
}