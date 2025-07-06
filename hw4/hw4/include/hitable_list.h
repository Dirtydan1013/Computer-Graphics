#ifndef HITABLE_LIST_H
#define HITABLE_LIST_H

#include "hitable.h"
#include <vector>

class hitable_list : public hitable {
public:
    hitable_list() {}
    hitable_list(hitable* object) { add(object); }

    void clear() { objects.clear(); }
    void add(hitable* object) { objects.push_back(object); }

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        float closest_so_far = t_max;

        for (const auto& obj : objects) {
            if (obj->hit(r, t_min, closest_so_far, temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }

public:
    std::vector<hitable*> objects;
};

#endif
