#ifndef BOX_HPP
#define BOX_HPP

#include "rtweekend.hpp"
#include "rectangle.hpp"

class box{
public:
    box() = default;
    box(const point& p0, const point& p1, material_t mat_type)
        : box_min { p0 }
        , box_max { p1 }
        , material_type { mat_type }
        {
            sides.emplace_back(xy_rect(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), mat_type));
            sides.emplace_back(xy_rect(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), mat_type));
            sides.emplace_back(xz_rect(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), mat_type));
            sides.emplace_back(xz_rect(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), mat_type));
            sides.emplace_back(yz_rect(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), mat_type));
            sides.emplace_back(yz_rect(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), mat_type));
        }
    
    bool hit(const ray& r, real_t min, real_t max, hit_record& rec, material_t& hit_material_type) const
    {
        hit_record temp_rec;
        material_t temp_material_type;
        auto hit_anything = false;
        auto closest_so_far = max;
        // Checking if the ray hits any of the spheres
        for (const auto& side : sides) {
            if (std::visit([&](auto&& arg) { return arg.hit(r, min, closest_so_far, temp_rec, temp_material_type); }, side)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
                hit_material_type = temp_material_type;
            }
        }
        return hit_anything;
    }
    
    point box_min;
    point box_max;
    material_t material_type;
    std::vector<rectangle_t> sides;
};

#endif