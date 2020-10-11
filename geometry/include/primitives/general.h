#pragma once

#include <iostream>
#include <limits>
#include <cmath>
#include <array>

namespace ivkg {
    const long double tolerance = 1e-5;

    using coordinate_t = long double;

    bool valid(const coordinate_t& coordinate) {
        return !(std::isnan(coordinate) || !std::isfinite(coordinate));
    }

    bool equal(const coordinate_t& lhs, const coordinate_t& rhs) {
        return valid(lhs) && valid(rhs) && (std::abs(lhs - rhs) < tolerance);
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
}