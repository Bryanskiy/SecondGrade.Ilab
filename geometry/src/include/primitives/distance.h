#pragma once

#include "general.h"

namespace lingeo {
    long double signed_distance(const point_t<3>& point, const plane_t& plane);
    long double signed_distance(const plane_t& plane, const point_t<3>& point);

    long double distance(const line_t<3>& lhs, const line_t<3>& rhs);
}

long double lingeo::signed_distance(const lingeo::point_t<3>& point, const lingeo::plane_t& plane) {
    if(!point.valid() || !plane.valid()) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    vector_t<3> plane_normal = plane.normal();
    plane_normal.normalize();

    point_t<3> p = plane.get_point();
    return dot(plane_normal, vector_t<3>{p, point});
}

long double lingeo::signed_distance(const plane_t& plane, const point_t<3>& point) {
    return signed_distance(point, plane);
}

long double lingeo::distance(const lingeo::line_t<3>& lhs, const lingeo::line_t<3>& rhs) {
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