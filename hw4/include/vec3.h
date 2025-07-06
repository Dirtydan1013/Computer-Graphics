#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <cstdlib>
#include <iostream>


class vec3 {
public:
    float e[3];

    vec3() : e{0,0,0} {}
    vec3(float e0, float e1, float e2) : e{e0,e1,e2} {}

    float x() const { return e[0]; }
    float y() const { return e[1]; }
    float z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    float operator[](int i) const { return e[i]; }
    float& operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3 &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }
    vec3& operator*=(float t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }
    vec3& operator/=(float t) {
        return *this *= 1/t;
    }

    float length() const {
        return std::sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    }
    float length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

   
    bool near_zero() const {
        const auto s = 1e-8f;
        return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
    }

    static vec3 random() {
        return vec3(
            std::rand() / (RAND_MAX + 1.0f),
            std::rand() / (RAND_MAX + 1.0f),
            std::rand() / (RAND_MAX + 1.0f)
        );
    }

    static vec3 random(float min, float max) {
        return vec3(
            min + (max-min)*(std::rand()/(RAND_MAX+1.0f)),
            min + (max-min)*(std::rand()/(RAND_MAX+1.0f)),
            min + (max-min)*(std::rand()/(RAND_MAX+1.0f))
        );
    }

    // 反射
    static vec3 reflect(const vec3& v, const vec3& n);
    // 折射
    static bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted);
    // Schlick 近似
    static float schlick(float cosine, float ref_idx);
};

using point3 = vec3;  
using color  = vec3;   

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.x()+v.x(), u.y()+v.y(), u.z()+v.z());
}
inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.x()-v.x(), u.y()-v.y(), u.z()-v.z());
}
inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.x()*v.x(), u.y()*v.y(), u.z()*v.z());
}
inline vec3 operator*(float t, const vec3 &v) {
    return vec3(t*v.x(), t*v.y(), t*v.z());
}
inline vec3 operator*(const vec3 &v, float t) {
    return t * v;
}
inline vec3 operator/(vec3 v, float t) {
    return (1/t) * v;
}

inline float dot(const vec3 &u, const vec3 &v) {
    return u.x()*v.x() + u.y()*v.y() + u.z()*v.z();
}
inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(
        u.y()*v.z() - u.z()*v.y(),
        u.z()*v.x() - u.x()*v.z(),
        u.x()*v.y() - u.y()*v.x()
    );
}
inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

//------------------------------------------------------------------------------
// vec3::reflect / refract / schlick 
//------------------------------------------------------------------------------
inline vec3 vec3::reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v,n)*n;
}

inline bool vec3::refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
    vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0f - ni_over_nt*ni_over_nt*(1 - dt*dt);
    if (discriminant > 0) {
        refracted = ni_over_nt*(uv - n*dt) - n*std::sqrt(discriminant);
        return true;
    }
    return false;
}

inline float vec3::schlick(float cosine, float ref_idx) {
    float r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0)*std::pow((1 - cosine), 5);
}

#endif // VEC3_H
