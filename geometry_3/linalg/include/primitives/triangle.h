#pragma once

#include "general.h"

namespace lingeo {

    template<std::size_t dim_>
    class triangle_t {
    public:
        triangle_t() = default;
        triangle_t(const triangle_t<dim_>&) = default;
        triangle_t(std::initializer_list<point_t<dim_>> points);

        point_t<dim_>& operator[](std::size_t idx);
        const point_t<dim_>& operator[](std::size_t idx) const;
        bool operator==(const triangle_t<dim_>& rhs) const;

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
lingeo::triangle_t<dim_>::triangle_t(std::initializer_list<point_t < dim_>> points) {
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
lingeo::point_t<dim_>& lingeo::triangle_t<dim_>::operator[](std::size_t idx) {
    return points_[idx];
}

template<std::size_t dim_>
bool lingeo::triangle_t<dim_>::operator==(const triangle_t<dim_>& rhs) const {
    if(points_.size() != rhs.points_.size()) {
        return false;
    }

    for(std::size_t i = 0, max = points_.size(); i < max;++i) {
        if(!(points_[i] == rhs.points_[i])) {
            return false;
        }
    }
    return true;
}

template<std::size_t dim_>
const lingeo::point_t<dim_>& lingeo::triangle_t<dim_>::operator[](std::size_t idx) const{
    return points_[idx];
}

template<std::size_t dim_>
bool lingeo::triangle_t<dim_>::valid() const {
    return points_[0].valid() && points_[1].valid() && points_[2].valid();
}

template<std::size_t dim_>
std::pair<long double, long double> lingeo::triangle_t<dim_>::projection_i(std::size_t i) const {
    std::pair<long double, long double> ret;
    ret.first = std::min(std::min(points_[0][i], points_[1][i]), points_[2][i]);
    ret.second = std::max(std::max(points_[0][i], points_[1][i]), points_[2][i]);
    return ret;
}


template <std::size_t DIM_>
std::istream& lingeo::operator>>(std::istream& in, lingeo::triangle_t<DIM_>& triangle) {
    for(std::size_t i = 0; i < DIM_; ++i) {
        if(!in.good()) {
            return in;
        }
        in >> triangle[i];
    }

    return in;
}

template <std::size_t DIM_>
std::ostream& lingeo::operator<<(std::ostream& in, const triangle_t<DIM_>& triangle) {
    for(std::size_t i = 0; i < DIM_; ++i) {
        in << triangle[i] << ' ';
    }

    return in;
}


