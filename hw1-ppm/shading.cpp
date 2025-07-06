#include <iostream>
#include <fstream>
#include <cmath>
#include "vec3.h"
#include "ray.h"


float hit_sphere(const vec3 &center, float radius, const ray& r) {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) return -1.0; 
    return (-b - sqrt(discriminant)) / (2.0 * a); 
}


vec3 color(const ray& r, const vec3& light_source) {
    float t = hit_sphere(vec3(0, 0, -1), 0.5, r); 
    if (t > 0.0) {
        vec3 P = r.point_at_parameter(t);   
        vec3 N = unit_vector(P - vec3(0, 0, -1)); 
        vec3 L = unit_vector(light_source - P); 

        
        float diffuse = std::max(0.0f, dot(N, L));

        return vec3(1, 1, 1) * diffuse; 
    }

    
    vec3 unit_direction = unit_vector(r.direction());
    float t_background = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t_background) * vec3(1, 1, 1) + t_background * vec3(0.5, 0.7, 1.0);
}

int main() {
    
    int single_width = 200; 
    int height = 100; 
    int width = single_width * 3; 

    std::ofstream file("shading.ppm");
    file << "P3\n" << width << " " << height << "\n255\n";

    
    vec3 lower_left_corner(-2, -1, -1);
    vec3 horizontal(4, 0, 0);
    vec3 vertical(0, 2, 0);
    vec3 origin(0, 0, 0);

    
    vec3 light_sources[3] = {
        vec3(-1, 1, 0), 
        vec3(0, 0, 0),  
        vec3(1, 1, 0)  
    };

    
    for (int j = height - 1; j >= 0; j--) {
        for (int scene = 0; scene < 3; scene++) { 
            for (int i = 0; i < single_width; i++) {
                float u = float(i) / float(single_width);
                float v = float(j) / float(height);
                vec3 uvcenter = lower_left_corner + u * horizontal + v * vertical;
                ray r(origin, uvcenter - origin);
                vec3 col = color(r, light_sources[scene]); 

                int ir = int(255.99 * col.x());
                int ig = int(255.99 * col.y());
                int ib = int(255.99 * col.z());

                file << ir << " " << ig << " " << ib << "\n";
            }
        }
    }

    file.close();
    return 0;
}
