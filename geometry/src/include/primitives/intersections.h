#pragma once

#include "general.h"

namespace ivkg {
    line_t<3> intersection(const plane_t& lhs, const plane_t& rhs);
    bool intersection(const triangle_t<3>& lhs, const triangle_t<3>& rhs);
    bool intersection(const segment_t<3>& lhs, const segment_t<3>& rhs);
    bool intersection(const segment_t<3>& s, const point_t<3>& p);
}

ivkg::line_t<3> ivkg::intersection(const ivkg::plane_t& lhs, const ivkg::plane_t& rhs) {
    return lhs.intersect((rhs));
}

bool ivkg::intersection(const segment_t<3>& s, const point_t<3>& p) {
    long double t[3] = {0.0, 0.0, 0.0};
    for(std::size_t i = 0; i < 3lu; ++i) {
        if(!equal(s.get_scd()[i], 0.0))
            t[i] = (p[i] - s.get_fst()[i]) / s.get_scd()[i];
    }

    bool flag = true;
    for(std::size_t i = 0; i < 3lu; ++i) {
        flag = flag && (p[i] == s.get_fst()[i] + t[i] * s.get_scd()[i]);
    }

    return flag;
}

bool ivkg::intersection(const segment_t<3>& lhs, const segment_t<3>& rhs) {
    vector_t<3> u{rhs.get_fst(), lhs.get_fst()};

    long double a = dot(vector_t<3>{lhs.get_fst(), lhs.get_scd()}, vector_t<3>{lhs.get_fst(), lhs.get_scd()});
    long double b = dot(vector_t<3>{lhs.get_fst(), lhs.get_scd()}, vector_t<3>{rhs.get_fst(), rhs.get_scd()});
    long double c = dot(vector_t<3>{rhs.get_fst(), rhs.get_scd()}, vector_t<3>{rhs.get_fst(), rhs.get_scd()});
    long double d = dot(vector_t<3>{lhs.get_fst(), lhs.get_scd()}, u);
    long double e = dot(vector_t<3>{rhs.get_fst(), rhs.get_scd()}, u);
    long double f = dot(u, u);

    long double det = a * c - b * b;

    long double s, t;
    if(equal(det, 0.0)) {
        return intersection(lhs, rhs.get_fst()) && intersection(lhs, rhs.get_scd()) &&
               intersection(rhs, lhs.get_fst()) && intersection(lhs, rhs.get_scd());
    }

    else {
        long double invdet = 1.0 / det;

        s = (b * e - c * d) * invdet;
        t = (a * e - b * d) * invdet;
    }

    if(s > 1 || s < 0 || t > 1 || t < 0) {
        return false;
    }

    vector_t<3> lhs_P = lhs.get_start() + s * lhs.get_direction();
    vector_t<3> rhs_P = rhs.get_start() + t * rhs.get_direction();

    if(equal((lhs_P - rhs_P).len(), 0.0)) {
        return true;
    }

    return false;
}


bool ivkg::intersection(const triangle_t<3>& lhs, const triangle_t<3>& rhs) {
    if(!lhs.valid() || !rhs.valid()) {
        return false;
    }

    plane_t lhs_plane{lhs[0], lhs[1], lhs[2]};
    long double dist[2][3];
    for(std::size_t i = 0; i < 3; ++i) {
        dist[1][i] = signed_distance(rhs[i], lhs_plane);
    }


    if((sign(dist[1][0]) == sign(dist[1][1])) && (sign(dist[1][0]) == sign(dist[1][2])) && (sign(dist[1][0]) != 0)) {
        return false;
    }

    plane_t rhs_plane{rhs[0], rhs[1], rhs[2]};
    if(parallel(lhs_plane, rhs_plane)) {
        if(lhs_plane == rhs_plane) {
            segment_t<3> lhs_AB{lhs[0], lhs[1]};
            segment_t<3> lhs_BC{lhs[1], lhs[2]};
            segment_t<3> lhs_AC(lhs[0], lhs[2]);

            segment_t<3> rhs_AB{rhs[0], rhs[1]};
            segment_t<3> rhs_BC{rhs[1], rhs[2]};
            segment_t<3> rhs_AC(rhs[0], rhs[2]);

            return intersection(lhs_AB, rhs_AB) ||
                   intersection(lhs_AB, rhs_BC) ||
                   intersection(lhs_AB, rhs_AC) ||
                   intersection(lhs_BC, rhs_AB) ||
                   intersection(lhs_BC, rhs_BC) ||
                   intersection(lhs_BC, rhs_AC) ||
                   intersection(lhs_AC, rhs_AB) ||
                   intersection(lhs_AC, rhs_BC) ||
                   intersection(lhs_AC, rhs_AC);
        }

        return false;
    }

    for(std::size_t i = 0; i < 3; ++i) {
        dist[0][i] = signed_distance(lhs[i], rhs_plane);
    }


    if((sign(dist[0][0]) == sign(dist[0][1])) && (sign(dist[0][0]) == sign(dist[0][2])) && (sign(dist[0][0]) != 0)) {
        return false;
    }

    line_t<3> intersection_line = intersection(lhs_plane, rhs_plane);

    vector_t<3> dir = intersection_line.get_direction();
    vector_t<3> start = intersection_line.get_start();
    point_t<3> start_point{start[0], start[1], start[2]};

    long double projections[2][3];
    for(std::size_t i = 0; i < 3; ++i) {
        projections[0][i] = dot(dir, vector_t<3>{start_point, lhs[i]});
        projections[1][i] = dot(dir, vector_t<3>{start_point, rhs[i]});
    }

    if((sign(dist[0][0]) != sign(dist[0][1])) && sign((dist[0][0]) != sign(dist[0][2]))) {
        swap(dist[0][0], dist[0][2]);
        swap(projections[0][0], projections[0][2]);
    } else if((sign(dist[0][1]) != sign(dist[0][0])) && sign((dist[0][1]) != sign(dist[0][2]))) {
        swap(dist[0][1], dist[0][2]);
        swap(projections[0][1], projections[0][2]);
    }

    if((sign(dist[1][0]) != sign(dist[1][1])) && sign((dist[1][0]) != sign(dist[1][2]))) {
        swap(dist[1][0], dist[1][2]);
        swap(projections[1][0], projections[1][2]);
    } else if((sign(dist[1][1]) != sign(dist[1][0])) && sign((dist[1][1]) != sign(dist[1][2]))) {
        swap(dist[1][1], dist[1][2]);
        swap(projections[1][1], projections[1][2]);
    }

    long double t[2][2];
    for(std::size_t i = 0; i < 2; ++i) {
        t[0][i] = projections[0][i] + (projections[0][2] - projections[0][i]) * (dist[0][i] / (dist[0][i] - dist[0][2]));
        t[1][i] = projections[1][i] + (projections[1][2] - projections[1][i]) * (dist[1][i] / (dist[1][i] - dist[1][2]));
    }

    long double max0 = std::max(t[0][0], t[0][1]);
    long double min0 = std::min(t[0][0], t[0][1]);

    long double max1 = std::max(t[1][0], t[1][1]);
    long double min1 = std::min(t[1][0], t[1][1]);

    if(max0 < min1 || max1 < min0) {
        return false;
    }

    return true;
}
