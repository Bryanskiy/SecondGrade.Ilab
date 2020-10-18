#pragma once

#include "general.h"

namespace lingeo {
    template<std::size_t dim_>
    class point_t {

    public:
        point_t();
        point_t(const point_t<dim_> &) = default;
        point_t(std::initializer_list <coordinate_t> coordinates);
        point_t(const vector_t<dim_>& vec);
        ~point_t() = default;

        bool valid() const;
        bool operator==(const point_t<dim_>& rhs) const;

        coordinate_t &operator[](std::size_t idx);
        const coordinate_t &operator[](std::size_t idx) const;

        long double distance(const point_t<dim_>& rhs) const;

    private:
        std::array <coordinate_t, dim_> coordinates_;
    };

    template<std::size_t DIM_>
    std::istream &operator>>(std::istream &in, point_t<DIM_> &point);

    template<std::size_t DIM_>
    std::ostream &operator<<(std::ostream &out, const point_t<DIM_> &point);
}

/* ------------------------------------------------
                REALIZATION_POINT_CLASS
-------------------------------------------------*/

template<std::size_t DIM_>
std::istream& lingeo::operator>>(std::istream& in, lingeo::point_t<DIM_>& point) {
    for(std::size_t i = 0; i < DIM_; ++i) {
        in >> point[i];
    }

    return in;
}

template<std::size_t DIM_>
std::ostream& lingeo::operator<<(std::ostream& out, const lingeo::point_t<DIM_>& point) {
    out << '(';
    out << point[0];
    for(std::size_t i = 1; i < DIM_; ++i) {
        out << ", ";
        out << point[i];
    }

    out << ')';
    return out;
}

template<std::size_t dim_>
lingeo::point_t<dim_>::point_t() {
    for(coordinate_t& elem : coordinates_) {
        elem = std::numeric_limits<double>::quiet_NaN();
    }
}

template<std::size_t dim_>
lingeo::point_t<dim_>::point_t(std::initializer_list<coordinate_t> coordinates) {
    if(coordinates.size() != dim_) {
        return;
    }

    auto obj_iter  = coordinates_.begin();
    auto init_iter = coordinates.begin();
    while (obj_iter != coordinates_.end()) {
        *obj_iter = *init_iter;
        obj_iter++;
        init_iter++;
    }
}

template<std::size_t dim_>
lingeo::point_t<dim_>::point_t(const lingeo::vector_t<dim_>& vec) {
    for(std::size_t i = 0; i < dim_; ++i) {
        coordinates_[i] = vec[i];
    }
}

template<std::size_t dim_>
bool lingeo::point_t<dim_>::valid() const {
    for(const coordinate_t& elem : coordinates_) {
        if(!lingeo::valid(elem))
            return false;
    }

    return true;
}
template<std::size_t dim_>
bool lingeo::point_t<dim_>::operator==(const point_t<dim_>& rhs) const {
    bool flag = true;

    auto rhs_iter = rhs.coordinates_.begin();
    for(const auto& lhs_coordinate : coordinates_) {
        flag = flag && equal(lhs_coordinate, *rhs_iter);
        ++rhs_iter;
    }

    return flag;
}

template<std::size_t dim_>
lingeo::coordinate_t& lingeo::point_t<dim_>::operator[](std::size_t idx) {
    return coordinates_[idx];
}

template<std::size_t dim_>
const lingeo::coordinate_t& lingeo::point_t<dim_>::operator[](std::size_t idx) const {
    return coordinates_[idx];
}

template<std::size_t dim_>
long double lingeo::point_t<dim_>::distance(const point_t<dim_>& rhs) const {
    return lingeo::vector_t<dim_>(rhs, *this).len();
}
