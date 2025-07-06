#include <iostream>
#include <fstream>
#include "vec3.h"
#include "ray.h"

// ✅ 檢測光線是否與球體相交
bool hit_sphere(const vec3 &center, float radius, const ray& r)
{
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    
    return (discriminant > 0); // 如果判別式 > 0，表示光線與球體有交點
}

// ✅ 根據光線決定像素顏色
vec3 color(const ray& r)
{
    if (hit_sphere(vec3(0, 0, -1), 0.5, r)) {
        return vec3(1, 0, 0); // 如果光線打到球體，顯示紅色
    }
    
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * vec3(1, 1, 1) + t * vec3(0.5, 0.7, 1.0); // 天空背景
}

int main()
{
    // ✅ 設定影像解析度
    int width = 200;
    int height = 100;
    std::ofstream file("red_sphere.ppm");
    file << "P3\n" << width << " " << height << "\n255\n";

    // ✅ 設定攝影機參數
    vec3 lower_left_corner(-2, -1, -1);
    vec3 horizontal(4, 0, 0);
    vec3 vertical(0, 2, 0);
    vec3 origin(0, 0, 0);

    // ✅ 逐像素計算顏色
    for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            float u = float(i) / float(width);
            float v = float(j) / float(height);
            vec3 uvcenter = lower_left_corner + u * horizontal + v * vertical;
            ray r(origin, uvcenter - origin);
            vec3 col = color(r); // 計算該像素的顏色

            int ir = int(255.99 * col.x());
            int ig = int(255.99 * col.y());
            int ib = int(255.99 * col.z());

            file << ir << " " << ig << " " << ib << "\n";
        }
    }

    file.close();

    return 0;
}
