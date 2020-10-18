#pragma once

#include "general.h"

namespace lingeo {

    template<std::size_t dim_>
    class line_t {
    public:
        line_t() = default;
        line_t(const line_t<dim_> &) = default;
        line_t(const vector_t<dim_> &lhs, const vector_t<dim_> &rhs);
        line_t(const point_t<dim_> &lhs, const point_t<dim_> &rhs);

        vector_t<dim_> get_direction() const;
        vector_t<dim_> get_start() const;

        bool valid() const;

    private:
        vector_t<dim_> start_;
        vector_t<dim_> direction_;
    };

    template<std::size_t DIM_>
    bool operator==(const line_t<DIM_> &lhs, const line_t<DIM_> &rhs);

    template<std::size_t DIM_>
    bool parallel(const line_t<DIM_> &lhs, const line_t<DIM_> &rhs);

}

/* ------------------------------------------------
                REALIZATION_LINE_CLASS
-------------------------------------------------*/

template<std::size_t dim_>
lingeo::line_t<dim_>::line_t(const lingeo::vector_t<dim_>& lhs, const lingeo::vector_t<dim_>& rhs) :
        start_{lhs}, direction_{rhs} {}

template<std::size_t dim_>
lingeo::line_t<dim_>::line_t(const lingeo::point_t<dim_>& lhs, const lingeo::point_t<dim_>& rhs) : start_{lhs} {
    for(std::size_t i = 0; i < dim_; ++i) {
        direction_[i] = rhs[i] - lhs[i];
    }
}

template<std::size_t dim_>
bool lingeo::line_t<dim_>::valid() const {
    return start_.valid() && direction_.valid();
}

template<std::size_t dim_>
lingeo::vector_t<dim_> lingeo::line_t<dim_>::get_direction() const {
    return direction_;
}

template<std::size_t dim_>
lingeo::vector_t<dim_> lingeo::line_t<dim_>::get_start() const {
    return start_;
}

template<std::size_t DIM_>
bool lingeo::operator==(const lingeo::line_t<DIM_>& lhs, const lingeo::line_t<DIM_>& rhs) {
    vector_t<DIM_> tmp = rhs.get_start() - lhs.get_start();
    return parallel(tmp, lhs.get_direction());
}

template<std::size_t DIM_>
bool lingeo::parallel(const lingeo::line_t<DIM_>& lhs, const lingeo::line_t<DIM_>& rhs) {
    return parallel(lhs.get_direction(), rhs.get_direction());
}