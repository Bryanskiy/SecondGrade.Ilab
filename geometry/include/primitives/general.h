#pragma once

#include <iostream>
#include <limits>
#include <cmath>
#include <array>

namespace ivkg {
    const long double tolerance = 1e-5;

    using coordinate_t = long double;

    inline bool valid(const coordinate_t& coordinate) {
        return !(std::isnan(coordinate) || !std::isfinite(coordinate));
    }

    inline bool equal(const coordinate_t& lhs, const coordinate_t& rhs) {
        return valid(lhs) && valid(rhs) && (std::abs(lhs - rhs) < tolerance);
    }

    inline int sign(long double x) {
        if(x > 0) {return 1;}
        if(x < 0) {return -1;}
        else {return 0;}
    }

    void swap(long double& lhs, long double& rhs) {
        long double tmp = lhs;
        lhs = rhs;
        rhs = tmp;
    }

    template<std::size_t dim_>
    class point_t;

    template<std::size_t dim_>
    class vector_t;

    template<std::size_t dim_>
    class line_t;

    template<std::size_t dim_>
    class segment_t;

    template<std::size_t dim_>
    class triangle_t;

    template<std::size_t dim_>
    void swap(point_t<dim_>& lhs, point_t<dim_>& rhs) {
        point_t<dim_> tmp = lhs;
        lhs = rhs;
        rhs = tmp;
    };
}