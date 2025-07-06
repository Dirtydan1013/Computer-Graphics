#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include "vec3.h"
#include "ray.h"


inline float random_float() {
    return rand() / (RAND_MAX + 1.0);
}


float hit_sphere(const vec3 &center, float radius, const ray& r) {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) return -1.0; 
    return (-b - sqrt(discriminant)) / (2.0 * a); 
}


vec3 color(const ray& r, const std::vector<std::pair<vec3, vec3>>& lights) {
    float t = hit_sphere(vec3(0, 0, -1), 0.5, r); 
    if (t > 0.0) {
        vec3 P = r.point_at_parameter(t);   
        vec3 N = unit_vector(P - vec3(0, 0, -1)); 
        vec3 total_light(0, 0, 0); 

        
        for (const auto& light : lights) {
            vec3 L = unit_vector(light.first - P); 
            float diffuse = std::max(0.0f, dot(N, L)); 
            total_light += light.second * diffuse; 
        }

        return total_light; 
    }

    
    vec3 unit_direction = unit_vector(r.direction());
    float t_background = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t_background) * vec3(1, 1, 1) + t_background * vec3(0.5, 0.7, 1.0);
}

int main() {
  
    int width = 400;
    int height = 200;
    int samples_per_pixel = 100; 
    std::ofstream file("multilight.ppm");
    file << "P3\n" << width << " " << height << "\n255\n";

   
    vec3 lower_left_corner(-2, -1, -1);
    vec3 horizontal(4, 0, 0);
    vec3 vertical(0, 2, 0);
    vec3 origin(0, 0, 0);

    
    std::vector<std::pair<vec3, vec3>> lights = {
        { vec3(-1, 1, 0), vec3(1, 1, 1) }, 
        { vec3(1, 1, 0), vec3(1, 0, 0) },  
        { vec3(0, 1, 1), vec3(0, 0, 1) }   
    };

    
    for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            vec3 col(0, 0, 0); 
            for (int s = 0; s < samples_per_pixel; s++) {
                float u = (i + random_float()) / float(width);
                float v = (j + random_float()) / float(height);
                vec3 uvcenter = lower_left_corner + u * horizontal + v * vertical;
                ray r(origin, uvcenter - origin);
                col += color(r, lights); 
            }
            col /= float(samples_per_pixel); 
            col = vec3(sqrt(col.x()), sqrt(col.y()), sqrt(col.z()));

            int ir = int(255.99 * col.x());
            int ig = int(255.99 * col.y());
            int ib = int(255.99 * col.z());

            file << ir << " " << ig << " " << ib << "\n";
        }
    }

    file.close();
    return 0;
}
