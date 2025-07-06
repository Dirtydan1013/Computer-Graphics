#ifndef HITABLE_H
#define HITABLE_H

#include "ray.h"
#include "vec3.h"

class material;  // forward

struct hit_record {
    vec3 p;
    vec3 normal;
    float t;
    bool front_face;
    material* mat_ptr;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hitable {
public:
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

#endif
