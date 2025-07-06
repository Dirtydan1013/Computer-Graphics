#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
#include "utility.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


class camera {
public:
    camera(
        vec3 lookfrom,
        vec3 lookat,
        vec3   vup,
        float vfov,           
        float aspect_ratio,
        float aperture,
        float focus_dist
    ) {
        lens_radius = aperture / 2;
        float theta = degrees_to_radians(vfov);
        float h = tan(theta/2);
        float viewport_height = 2.0f * h;
        float viewport_width  = aspect_ratio * viewport_height;

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        origin = lookfrom;
        horizontal = focus_dist * viewport_width * u;
        vertical   = focus_dist * viewport_height * v;
        lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;
    }

    ray get_ray(float s, float t) const {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u*rd.x() + v*rd.y();
        return ray(
            origin + offset,
            lower_left_corner + s*horizontal + t*vertical - origin - offset
        );
    }

private:
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    float lens_radius;

    
    static float degrees_to_radians(float deg) {
        return deg * M_PI / 180.0f;
    }
};

#endif
