#pragma once

#include "general.h"

namespace ivkg {

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
ivkg::line_t<dim_>::line_t(const ivkg::vector_t<dim_>& lhs, const ivkg::vector_t<dim_>& rhs) :
        start_{lhs}, direction_{rhs} {}

template<std::size_t dim_>
ivkg::line_t<dim_>::line_t(const ivkg::point_t<dim_>& lhs, const ivkg::point_t<dim_>& rhs) : start_{lhs} {
    for(std::size_t i = 0; i < dim_; ++i) {
        direction_[i] = rhs[i] - lhs[i];
    }
}

template<std::size_t dim_>
bool ivkg::line_t<dim_>::valid() const {
    return start_.valid() && direction_.valid();
}

template<std::size_t dim_>
ivkg::vector_t<dim_> ivkg::line_t<dim_>::get_direction() const {
    return direction_;
}

template<std::size_t dim_>
ivkg::vector_t<dim_> ivkg::line_t<dim_>::get_start() const {
    return start_;
}

template<std::size_t DIM_>
bool ivkg::operator==(const ivkg::line_t<DIM_>& lhs, const ivkg::line_t<DIM_>& rhs) {
    vector_t<DIM_> tmp = rhs.get_start() - lhs.get_start();
    return parallel(tmp, lhs.get_direction());
}

template<std::size_t DIM_>
bool ivkg::parallel(const ivkg::line_t<DIM_>& lhs, const ivkg::line_t<DIM_>& rhs) {
    return parallel(lhs.get_direction(), rhs.get_direction());
}