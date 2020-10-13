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

            bool valid() const;

        private:
        std::array<point_t<dim_>, 3> points_;
    };
    template <std::size_t DIM_>
    std::istream& operator>>(std::istream& in, triangle_t<DIM_>& triangle);
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

template <std::size_t DIM_>
std::istream& ivkg::operator>>(std::istream& in, ivkg::triangle_t<DIM_>& triangle) {
    for(std::size_t i = 0; i < DIM_; ++i) {
        in >> triangle[i];
    }

    return in;
}


