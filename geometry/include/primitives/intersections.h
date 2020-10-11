#pragma once

#include "general.h"

namespace ivkg {
    line_t<3> intersection(const plane_t& lhs, const plane_t& rhs);
    bool intersection(const triangle_t<3>& tr, const plane_t& p);
    bool intersection(const triangle_t<3>& lhs, const triangle_t<3>& rhs);

}

ivkg::line_t<3> ivkg::intersection(const ivkg::plane_t& lhs, const ivkg::plane_t& rhs) {
    return lhs.intersect((rhs));
}

bool ivkg::intersection(const triangle_t<3>& lhs, const triangle_t<3>& rhs) {
    if(!lhs.valid() || !rhs.valid()) {
        return false;
    }

    ivkg::plane_t lhs_plane{lhs[0], lhs[1], lhs[2]};
    long double sdist1_1 = ivkg::signed_distance(rhs[0], lhs_plane);
    long double sdist1_2 = ivkg::signed_distance(rhs[1], lhs_plane);
    long double sdist1_3 = ivkg::signed_distance(rhs[2], lhs_plane);

    long double d1d2_1 = sdist1_1 * sdist1_2;
    long double d1d3_1 = sdist1_1 * sdist1_3;

    if(d1d2_1 > 0.0 && d1d3_1 > 0.0) {
        return false;
    }

    if(d1d2_1 < 0.0 && d1d3_1 < 0.0) {
        return false;
    }

    ivkg::plane_t rhs_plane{rhs[0], rhs[1], rhs[2]};
    if(parallel(lhs_plane, rhs_plane)) {
        if(lhs_plane == rhs_plane) {
            segment_t<3> lhs_AB{lhs[0], lhs[1]};
            segment_t<3> lhs_BC{lhs[1], lhs[2]};
            segment_t<3> lhs_AC(lhs[0], lhs[2]);

            segment_t<3> rhs_AB{rhs[0], rhs[1]};
            segment_t<3> rhs_BC{rhs[1], rhs[2]};
            segment_t<3> rhs_AC(rhs[0], rhs[2]);

            return equal(distance(lhs_AB, rhs_AB), 0.0) ||
                   equal(distance(lhs_AB, rhs_BC), 0.0) ||
                   equal(distance(lhs_AB, rhs_AC), 0.0) ||
                   equal(distance(lhs_BC, rhs_AB), 0.0) ||
                   equal(distance(lhs_BC, rhs_BC), 0.0) ||
                   equal(distance(lhs_BC, rhs_AC), 0.0) ||
                   equal(distance(lhs_AC, rhs_AB), 0.0) ||
                   equal(distance(lhs_AC, rhs_BC), 0.0) ||
                   equal(distance(lhs_AC, rhs_AC), 0.0);
        }

        return false;
    }

    long double sdist0_1 = ivkg::signed_distance(lhs[0], rhs_plane);
    long double sdist0_2 = ivkg::signed_distance(lhs[1], rhs_plane);
    long double sdist0_3 = ivkg::signed_distance(lhs[2], rhs_plane);

    long double d1d2_0 = sdist0_1 * sdist0_2;
    long double d1d3_0 = sdist0_1 * sdist0_3;

    if(d1d2_0 > 0.0 && d1d3_0 > 0.0) {
        return false;
    }

    if(d1d2_0 < 0.0 && d1d3_0 < 0.0) {
        return false;
    }

    ivkg::line_t<3> intersection_line = intersection(lhs_plane, rhs_plane);

    // lhs segment:
    ivkg::vector_t<3> dir = intersection_line.get_direction();
    ivkg::vector_t<3> start = intersection_line.get_start();

    long double V_0_0 = dot(dir, vector_t<3>{lhs[0], {0, 0, 0}} - start);
    long double V_0_1 = dot(dir, vector_t<3>{lhs[1], {0, 0, 0}} - start);
    long double V_0_2 = dot(dir, vector_t<3>{lhs[2], {0, 0, 0}} - start);

    long double t_0_0 = V_0_0 + (V_0_2 - V_0_0) * (sdist0_1 / (sdist0_1 - sdist0_3));
    long double t_0_1 = V_0_1 + (V_0_2 - V_0_1) * (sdist0_1 / (sdist0_1 - sdist0_3));

    //rhs segment:
    long double V_1_0 = dot(dir, vector_t<3>{rhs[0], {0, 0, 0}} - start);
    long double V_1_1 = dot(dir, vector_t<3>{rhs[1], {0, 0, 0}} - start);
    long double V_1_2 = dot(dir, vector_t<3>{rhs[2], {0, 0, 0}} - start);

    long double t_1_0 = V_1_0 + (V_1_2 - V_1_0) * (sdist1_1 / (sdist1_1 - sdist1_3));
    long double t_1_1 = V_1_1 + (V_1_2 - V_1_1) * (sdist1_1 / (sdist1_1 - sdist1_3));

    long double max1 = std::max(t_1_0, t_1_1);
    long double min1 = std::min(t_1_0, t_1_1);

    long double max0 = std::max(t_0_0, t_0_1);
    long double min0 = std::min(t_0_0, t_0_1);

    if(max1 < min0 || max0 < min1) {
        return false;
    }

    return true;
}

bool ivkg::intersection(const ivkg::triangle_t<3>& tr, const ivkg::plane_t& p) {
    if(!tr.valid() || !p.valid()) {
        return false;
    }

    long double dot1, dot2, dot3;
    ivkg::vector_t<3> plane_normal = p.normal();
    ivkg::vector_t<3> tmp = -p[3] / pow(plane_normal.len(), 2) * plane_normal;
    ivkg::point_t<3> plane_point{tmp[0], tmp[1], tmp[3]};

    dot1 = dot(plane_normal, vector_t<3>{plane_point, tr[0]});
    dot2 = dot(plane_normal, vector_t<3>{plane_point, tr[1]});
    dot3 = dot(plane_normal, vector_t<3>{plane_point, tr[2]});

    if (equal(dot1, 0.0)) dot1 = 0.0;
    if (equal(dot2, 0.0)) dot2 = 0.0;
    if (equal(dot3, 0.0)) dot3 = 0.0;

    long double d1d2 = dot1 * dot2;
    long double d1d3 = dot1 * dot3;

    if(d1d2 > 0.0 && d1d3 > 0.0) {
        return false;
    }

    if(d1d2 < 0.0 && d1d3 < 0.0) {
        return false;
    }

    return true;
}