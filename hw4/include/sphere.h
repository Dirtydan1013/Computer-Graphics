#ifndef SPHERE_H
#define SPHERE_H

#include "hitable.h"
#include <cmath>

class sphere : public hitable {
public:
    sphere() {}
    sphere(const vec3& cen, float r, material* m)
        : center(cen), radius(r), mat_ptr(m) {}

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override {
        vec3 oc = r.origin() - center;
        float a = dot(r.direction(), r.direction());
        float half_b = dot(oc, r.direction());
        float c = dot(oc, oc) - radius*radius;
        float discriminant = half_b*half_b - a*c;
        if (discriminant < 0) return false;
        float sqrtd = std::sqrt(discriminant);
        // 找到最近的 t
        float root = (-half_b - sqrtd) / a;
        if (root < t_min || root > t_max) {
            root = (-half_b + sqrtd) / a;
            if (root < t_min || root > t_max)
                return false;
        }
        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat_ptr = mat_ptr;
        return true;
    }

public:
    vec3 center;
    float radius;
    material* mat_ptr;
};

#endif
