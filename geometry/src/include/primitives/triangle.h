#pragma once

#include "general.h"

namespace ivkg {

    template<std::size_t dim_>
    class triangle_t {
    public:
        triangle_t() = default;
        triangle_t(const triangle_t<dim_>&) = default;
        triangle_t(std::initializer_list<point_t<dim_>> points);

        point_t<dim_>& operator[](std::size_t idx);
        const point_t<dim_>& operator[](std::size_t idx) const;

        std::pair<long double, long double> projection_i(std::size_t i) const;

        bool valid() const;

    private:
        std::array<point_t<dim_>, 3> points_;
    };
    template <std::size_t DIM_>
    std::istream& operator>>(std::istream& in, triangle_t<DIM_>& triangle);

    template <std::size_t DIM_>
    std::ostream& operator<<(std::ostream& in, const triangle_t<DIM_>& triangle);
}

template<std::size_t dim_>
ivkg::triangle_t<dim_>::triangle_t(std::initializer_list<point_t<dim_>> points) {
    if(points_.size() != points.size()) {
        return;
    }

    auto obj_iter  = points_.begin();
    auto init_iter = points.begin();
    while (obj_iter != points_.end()) {
        *obj_iter = *init_iter;
        obj_iter++;
        init_iter++;
    }
}

template<std::size_t dim_>
ivkg::point_t<dim_>& ivkg::triangle_t<dim_>::operator[](std::size_t idx) {
    return points_[idx];
}

template<std::size_t dim_>
const ivkg::point_t<dim_>& ivkg::triangle_t<dim_>::operator[](std::size_t idx) const{
    return points_[idx];
}

template<std::size_t dim_>
bool ivkg::triangle_t<dim_>::valid() const {
    return points_[0].valid() && points_[1].valid() && points_[2].valid();
}

template<std::size_t dim_>
std::pair<long double, long double> ivkg::triangle_t<dim_>::projection_i(std::size_t i) const {
    if(i > dim_) {
        return {0.0, 0.0};
    }
    std::pair<long double, long double> ret;
    ret.first = std::min(std::min(points_[0][i], points_[1][i]), points_[2][i]);
    ret.second = std::max(std::max(points_[0][i], points_[1][i]), points_[2][i]);
    return ret;
}


template <std::size_t DIM_>
std::istream& ivkg::operator>>(std::istream& in, ivkg::triangle_t<DIM_>& triangle) {
    for(std::size_t i = 0; i < DIM_; ++i) {
        in >> triangle[i];
    }

    return in;
}

template <std::size_t DIM_>
std::ostream& ivkg::operator<<(std::ostream& in, const triangle_t<DIM_>& triangle) {
    for(std::size_t i = 0; i < DIM_; ++i) {
        in << triangle[i] << ' ';
    }

    return in;
}


