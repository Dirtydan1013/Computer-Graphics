#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "utility.h"    // for random_in_unit_sphere(), random_double()

// ----------------------------------------------------------------------
// 抽象材质基类
// ----------------------------------------------------------------------
class material {
public:
    // 如果光线被散射，scatter 返回 true，并输出 attenuation & scattered
    virtual bool scatter(
        const ray& r_in,
        const hit_record& rec,
        vec3& attenuation,
        ray& scattered
    ) const = 0;
};

// ----------------------------------------------------------------------
// Lambertian（漫反射）
// ----------------------------------------------------------------------
class lambertian : public material {
public:
    lambertian(const vec3& a) : albedo(a) {}

    virtual bool scatter(
        const ray& r_in,
        const hit_record& rec,
        vec3& attenuation,
        ray& scattered
    ) const override {
        // 随机漫反射
        vec3 scatter_dir = rec.normal + random_unit_vector();
        // 避免 scatter_dir 太接近零向量
        if (scatter_dir.near_zero())
            scatter_dir = rec.normal;

        scattered = ray(rec.p, scatter_dir);
        attenuation = albedo;
        return true;
    }

public:
    vec3 albedo;
};

// ----------------------------------------------------------------------
// Metal
// ----------------------------------------------------------------------
class metal : public material {
public:
    metal(const vec3& a, float f) 
      : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(
        const ray& r_in,
        const hit_record& rec,
        vec3& attenuation,
        ray& scattered
    ) const override {
        vec3 reflected = vec3::reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
   
        return (dot(scattered.direction(), rec.normal) > 0);
    }

public:
    vec3 albedo;
    float fuzz;
};

// ----------------------------------------------------------------------
// Dielectric
// ----------------------------------------------------------------------
class dielectric : public material {
public:
    dielectric(float ri) : ref_idx(ri) {}

    virtual bool scatter(
        const ray& r_in,
        const hit_record& rec,
        vec3& attenuation,
        ray& scattered
    ) const override {
        attenuation = vec3(1.0f, 1.0f, 1.0f);  // 透明不吸收
        float etai_over_etat = rec.front_face ? (1.0f / ref_idx) : ref_idx;

        vec3 unit_dir = unit_vector(r_in.direction());
        float cos_theta = fmin(dot(-unit_dir, rec.normal), 1.0f);
        float sin_theta = std::sqrt(1.0f - cos_theta * cos_theta);

        bool cannot_refract = etai_over_etat * sin_theta > 1.0f;
        vec3 direction;

        if (cannot_refract 
            || vec3::schlick(cos_theta, etai_over_etat) > random_double())
        {
            direction = vec3::reflect(unit_dir, rec.normal);
        } else {
            vec3 refracted;
            vec3::refract(unit_dir, rec.normal, etai_over_etat, refracted);
            direction = refracted;
        }

        scattered = ray(rec.p, direction);
        return true;
    }

public:
    float ref_idx;  // 折射率
};

#endif // MATERIAL_H
