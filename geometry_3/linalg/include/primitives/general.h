
#pragma once

#include <iostream>
#include <limits>
#include <cmath>
#include <array>

namespace lingeo {
    const long double tolerance = 1e-8;

    using coordinate_t = long double;

    bool valid(const coordinate_t& coordinate);
    bool equal(const coordinate_t& lhs, const coordinate_t& rhs);
    int sign(long double x);
    void swap(long double& lhs, long double& rhs);

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