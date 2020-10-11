#pragma once

#include "general.h"

namespace ivkg {
    long double signed_distance(const point_t<3>& point, const plane_t& plane);
    long double signed_distance(const plane_t& plane, const point_t<3>& point);

    long double distance(const line_t<3>& lhs, const line_t<3>& rhs);
    long double distance(const segment_t<3>& lhs, const segment_t<3>& rhs);
}

long double ivkg::signed_distance(const ivkg::point_t<3>& point, const ivkg::plane_t& plane) {
    if(!point.valid() || !plane.valid()) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    vector_t<3> plane_normal = plane.normal();
    plane_normal.normalize();

    point_t<3> p = plane.get_point();
    return dot(plane_normal, vector_t<3>{p, point});
}

long double ivkg::signed_distance(const plane_t& plane, const point_t<3>& point) {
    return signed_distance(point, plane);
}

long double ivkg::distance(const ivkg::line_t<3>& lhs, const ivkg::line_t<3>& rhs) {
    vector_t<3> d0 = lhs.get_direction();
    vector_t<3> d1 = rhs.get_direction();
    vector_t<3> p0 = lhs.get_start();
    vector_t<3> p1 = rhs.get_start();

    vector_t<3> u = p0 - p1;

    long double a = dot(d0, d0);
    long double b = dot(d1, d0);
    long double c = dot(d1, d1);
    long double d = dot(d0, u);
    long double e = dot(d1, u);
    long double f = dot(u, u);

    long double det = a * c - b * b;

    long double s, t;
    if(equal(det, 0.0)) {
        s = 0;
        t = (b > c) ? d / b : e / c;
    }

    else {
        long double invdet = 1.0 / det;

        s = (b * e - c * d) * invdet;
        t = (-a * e + b * d) * invdet;
    }

    return std::sqrt(s * (a * s + b * t + 2.0 * d) + t * (b * s + c * t + 2.0 * e) + f);
}

long double ivkg::distance(const segment_t<3>& lhs, const segment_t<3>& rhs) {
    vector_t<3> u{rhs.get_fst(), lhs.get_fst()};

    long double a = dot(vector_t<3>{lhs.get_fst(), lhs.get_scd()}, vector_t<3>{lhs.get_fst(), lhs.get_scd()});
    long double b = dot(vector_t<3>{lhs.get_fst(), lhs.get_scd()}, vector_t<3>{rhs.get_fst(), rhs.get_scd()});
    long double c = dot(vector_t<3>{rhs.get_fst(), rhs.get_scd()}, vector_t<3>{rhs.get_fst(), rhs.get_scd()});
    long double d = dot(vector_t<3>{lhs.get_fst(), lhs.get_scd()}, u);
    long double e = dot(vector_t<3>{rhs.get_fst(), rhs.get_scd()}, u);

    long double det = a * c - b * b;

    long double snum, tnum;
    long double sdenom, tdenom;

    if(equal(det, 0.0)) {
        snum = 0; tnum = e; tdenom = c ; sdenom = det;
    } else {
        snum = b * e - c * d;
        tnum = a * e - b * d;
    }

    sdenom = det;
    if (snum < 0) {
        snum = 0, tnum = e, tdenom = c;
    } else if(snum > det) {
        snum = det, tnum = e + b, tdenom = c;
    } else {
        tdenom = det;
    }

    if (tnum < 0) {
        tnum = 0;
        if (-d < 0) {
            snum = 0;
        } else if (-d > a) {
            snum = sdenom;
        } else {
            snum = -d, sdenom = a;
        }
    } else if (tnum > tdenom) {
        tnum = tdenom;
        if ((-d + b) < 0) {
            snum = 0;
        } else if ((-d + b) > a) {
            snum = sdenom;
        } else {
            snum = -d + b, sdenom = a;
        }
    }

    long double s = snum / sdenom;
    long double t = tnum / tdenom;

    vector_t<3> v = vector_t<3>{lhs.get_fst()} + s * vector_t<3>{lhs.get_fst(), lhs.get_scd()} -
            vector_t<3>{rhs.get_fst()} + t * vector_t<3>{rhs.get_fst(), rhs.get_scd()};

    return std::sqrt(dot(v, v));
}