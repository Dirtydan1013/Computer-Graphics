
//  在終端執行以下指令
//  g++ -std=c++11 -O2 main.cpp -Iinclude -o raytracer
//  .\raytracer.exe | Out-File -FilePath out.ppm -Encoding ASCII


#include <iostream>
#include <cstdlib>
#include <ctime>

#include "utility.h"
#include "camera.h"
#include "hitable_list.h"
#include "sphere.h"
#include "material.h"

vec3 ray_color(const ray& r, const hitable& world, int depth) {
    hit_record rec;

    
    if (depth <= 0)
        return vec3(0,0,0);

    if (world.hit(r, 0.001f, Infinity, rec)) {
        ray scattered;
        vec3 attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth-1);
        return vec3(0,0,0);
    }

    
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5f * (unit_direction.y() + 1.0f);
    return (1.0f - t)*vec3(1.0f, 1.0f, 1.0f)
           + t*vec3(0.5f, 0.7f, 1.0f);
}

int main() {
    std::srand(std::time(nullptr));

    
    const float aspect_ratio = 16.0f / 9.0f;
    const int image_width = 800;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    
    hitable_list world;

    
    world.add(new sphere(
        vec3(0, -1000, 0), 1000,
        new lambertian(vec3(0.5f, 0.5f, 0.5f)))
    );

    
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = random_double();
            vec3 center(
                a + 0.9f*random_double(),
                0.2f,
                b + 0.9f*random_double()
            );

            if ((center - vec3(4,0.2,0)).length() > 0.9f) {
                material* sphere_mat;
                if (choose_mat < 0.8f) {
                    // 漫反射
                    vec3 albedo = vec3::random() * vec3::random();
                    sphere_mat = new lambertian(albedo);
                } else if (choose_mat < 0.95f) {
                    // 金属
                    vec3 albedo = vec3::random(0.5f,1.0f);
                    float fuzz = random_double(0, 0.5f);
                    sphere_mat = new metal(albedo, fuzz);
                } else {
                    // 玻璃
                    sphere_mat = new dielectric(1.5f);
                }
                world.add(new sphere(center, 0.2f, sphere_mat));
            }
        }
    }

    
    world.add(new sphere(vec3(0,1,0),   1.0f, new dielectric(1.5f)));
    world.add(new sphere(vec3(-4,1,0),  1.0f, new lambertian(vec3(0.4f,0.2f,0.1f))));
    world.add(new sphere(vec3(4,1,0),   1.0f, new metal(vec3(0.7f,0.6f,0.5f), 0.0f)));

    
    vec3 lookfrom(13,2,3);
    vec3 lookat(0,0,0);
    vec3 vup(0,1,0);
    float dist_to_focus = 10.0f;
    float aperture = 0.1f;
    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    // 渲染
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            vec3 pixel_color(0,0,0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                float u = (i + random_double()) / (image_width-1);
                float v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nDone.\n";
    return 0;
}
