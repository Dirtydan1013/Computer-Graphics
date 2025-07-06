#include <iostream>
#include <fstream>
#include "vec3.h"
#include "ray.h"


vec3 color(const ray& r) {
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5 * (unit_direction.y() + 1.0);

	 
	 vec3 sunset_color1(1.0, 0.6, 0.3); 
	 vec3 sunset_color2(0.2, 0.3, 0.7);  

     return (1.0 - t) * sunset_color1 + t * sunset_color2;
}

int main() {
    
    int width = 200;
    int height = 100;

    
    std::ofstream file("sky.ppm");
    file << "P3\n" << width << " " << height << "\n255\n";

   
    vec3 lower_left_corner(-2, -1, -1);
    vec3 horizontal(4, 0, 0);
    vec3 vertical(0, 2, 0);
    vec3 origin(0, 0, 0);

   
    for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            float u = float(i) / float(width);
            float v = float(j) / float(height);

            vec3 uvcenter = lower_left_corner + u * horizontal + v * vertical;
            ray r(origin, uvcenter - origin);

            vec3 col = color(r);

            int ir = int(255.99 * col.x());
            int ig = int(255.99 * col.y());
            int ib = int(255.99 * col.z());

            file << ir << " " << ig << " " << ib << "\n";
        }
    }

    file.close();

    return 0;
}
