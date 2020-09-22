#include <random>
#include <iostream>

#include "geometry.h"

bool intersect_triangles(const triangle_t& lhs, const triangle_t& rhs) {
    plane_t lhs_plane(lhs.A, lhs.B, lhs.C);
    plane_t rhs_plane(rhs.A, rhs.B, rhs.C);
    line_t plane_intersection = lhs_plane.intersect(rhs_plane);

    if(!plane_intersection.valid()) {
        segment_t lhs_AB(lhs.A, lhs.B);
        segment_t lhs_BC(lhs.B, lhs.C);
        segment_t rhs_AB(rhs.A, rhs.B);
        segment_t rhs_BC(rhs.B, rhs.C);

        return lhs_AB.intersect(rhs_AB).valid() || lhs_AB.intersect(rhs_BC).valid() ||
               lhs_BC.intersect(rhs_AB).valid() || lhs_BC.intersect(rhs_BC).valid();
    }

    bool lhs_line_intersection = lhs.include(plane_intersection);
    bool rhs_line_intersection = rhs.include(plane_intersection);

    return lhs_line_intersection && rhs_line_intersection;
}

point3D_t generate_random_point(double a, double b) {
    point3D_t ret;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(a, b);

    ret.x = dis(gen);
    ret.y = dis(gen);
    ret.z = dis(gen);

    return ret;
}

triangle_t generate_random_triangle(double a, double b) {
    triangle_t ret;
    ret.A = generate_random_point(a, b);
    ret.B = generate_random_point(a, b);
    ret.C = generate_random_point(a, b);

    return ret;
}

int main(){
    for(;;) {
        triangle_t fst = generate_random_triangle(-8.0, 7.0);
        triangle_t scd = generate_random_triangle(-8.0, 7.0);

        bool projection_metod = fst.intersect(scd);
        bool planes_metod = fst.intersect(scd);
        std::cout << "first: " << fst << " second: " << scd  <<
              "|" << projection_metod << ':' << planes_metod << std::endl;
        if(projection_metod != planes_metod) {
            exit(1);
        }
    }
}