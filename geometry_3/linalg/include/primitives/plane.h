#pragma once

#include "general.h"

namespace lingeo {

    class plane_t {
    public:
        plane_t();
        plane_t(const plane_t &) = default;
        plane_t(std::initializer_list<long double> coefficients);
        plane_t(const point_t<3>& x1, const point_t<3>& x2, const point_t<3>& x3);

        long double &operator[](std::size_t idx);
        const long double &operator[](std::size_t idx) const;
        point_t<3> get_point() const;

        vector_t<3> normal() const;

        bool valid() const;

        bool intersect(const point_t<3>& point) const;
        line_t<3> intersect(const plane_t &rhs) const;

    private:
        std::array<long double, 4> coefficients_;
    };

    bool operator==(const plane_t &lhs, const plane_t &rhs);
    bool parallel(const plane_t &lhs, const plane_t &rhs);
}
