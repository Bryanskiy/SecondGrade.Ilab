#pragma once

#include "general.h"

namespace ivkg {

    template<std::size_t dim_>
    class segment_t {
    public:
        segment_t() = default;
        segment_t(const segment_t<dim_>&) = default;
        segment_t(const point_t<dim_>& lhs, const point_t<dim_>& rhs);

        point_t<dim_> get_fst();
        const point_t<dim_> get_fst() const;

        point_t<dim_> get_scd();
        const point_t<dim_> get_scd() const;

        bool valid() const;

    private:
        point_t<dim_> fst_;
        point_t<dim_> scd_;
    };
}

template<std::size_t dim_>
ivkg::segment_t<dim_>::segment_t(const ivkg::point_t<dim_>& lhs, const ivkg::point_t<dim_>& rhs) :
    fst_{lhs}, scd_{rhs} {}

template<std::size_t dim_>
ivkg::point_t<dim_> ivkg::segment_t<dim_>::get_fst() {
    return fst_;
}

template<std::size_t dim_>
const ivkg::point_t<dim_> ivkg::segment_t<dim_>::get_fst() const{
    return fst_;
}

template<std::size_t dim_>
ivkg::point_t<dim_> ivkg::segment_t<dim_>::get_scd() {
    return scd_;
}

template<std::size_t dim_>
const ivkg::point_t<dim_> ivkg::segment_t<dim_>::get_scd() const{
    return scd_;
}

template<std::size_t dim_>
bool ivkg::segment_t<dim_>::valid() const {
    if(fst_ == scd_) {
        return false;
    }
    return fst_.valid() && scd_.valid();
}