#pragma once

#include "general.h"

namespace lingeo {

    template<std::size_t dim_>
    class vector_t {
    public:
        vector_t();
        vector_t(const vector_t<dim_> &) = default;
        vector_t(std::initializer_list<coordinate_t> coordinates);
        vector_t(const point_t<dim_> &lhs, const point_t<dim_> &rhs);
        vector_t(const point_t<dim_> &point);
        ~vector_t() = default;

        coordinate_t &operator[](std::size_t idx);
        const coordinate_t &operator[](std::size_t idx) const;

        vector_t<dim_> &operator+=(const vector_t<dim_> &rhs);
        vector_t<dim_> &operator-=(const vector_t<dim_> &rhs);
        vector_t<dim_> &operator*=(long double lambda);
        const vector_t<dim_> operator-();

        bool valid() const;

        long double len() const;
        void normalize();

        bool zero() const;

    private:
        std::array<coordinate_t, dim_> coordinates_;
    };

    template<std::size_t DIM_>
    long double dot(const vector_t<DIM_> &lhs, const vector_t<DIM_> &rhs);

    vector_t<3> cross(const vector_t<3> &lhs, const vector_t<3> &rhs);

    template<std::size_t DIM_>
    bool operator==(const vector_t<DIM_> &lhs, const vector_t<DIM_> &rhs);

    template<std::size_t DIM_>
    vector_t<DIM_> operator+(const vector_t<DIM_> &lhs, const vector_t<DIM_> &rhs);

    template<std::size_t DIM_>
    vector_t<DIM_> operator-(const vector_t<DIM_> &lhs, const vector_t<DIM_> &rhs);

    template<std::size_t DIM_>
    vector_t<DIM_> operator*(long double lambda, const vector_t<DIM_> &rhs);

    template<std::size_t DIM_>
    vector_t<DIM_> operator*(const vector_t<DIM_> &lhs, long double lambda);

    template<std::size_t DIM_>
    bool parallel(const vector_t<DIM_> &lhs, const vector_t<DIM_> &rhs);

    template<std::size_t DIM_>
    std::istream &operator>>(std::istream &in, vector_t<DIM_> &vector);

    /*
    template<std::size_t DIM_>
    std::ostream& operator<<(std::ostream& out, const vector_t<DIM_>& vector);
    */
}

/* ------------------------------------------------
                REALIZATION_VECTOR_CLASS
-------------------------------------------------*/
template<std::size_t dim_>
lingeo::vector_t<dim_>::vector_t() {
    for(coordinate_t& elem : coordinates_) {
        elem = std::numeric_limits<double>::quiet_NaN();
    }
}


//todo: avoid duplicate code (the same constructor in point class)
template<std::size_t dim_>
lingeo::vector_t<dim_>::vector_t(std::initializer_list<coordinate_t> coordinates) {
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

//todo: construct vector from points with different dimensions
template<std::size_t dim_>
lingeo::vector_t<dim_>::vector_t(const point_t<dim_>& lhs, const point_t<dim_>& rhs) {
    for(std::size_t i = 0; i < dim_; ++i) {
        coordinates_[i] = rhs[i] - lhs[i];
    }
}

template<std::size_t dim_>
lingeo::vector_t<dim_>& lingeo::vector_t<dim_>::operator+=(const vector_t<dim_>& rhs) {
    for(std::size_t i = 0; i < dim_; ++i) {
        coordinates_[i] += rhs[i];
    }

    return *this;
}

//todo: avoid duplicate code (the same in operator +=)
template<std::size_t dim_>
lingeo::vector_t<dim_>& lingeo::vector_t<dim_>::operator-=(const vector_t<dim_>& rhs) {
    for(std::size_t i = 0; i < dim_; ++i) {
        coordinates_[i] -= rhs[i];
    }

    return *this;
}

//todo: overflow protection
template<std::size_t dim_>
lingeo::vector_t<dim_>& lingeo::vector_t<dim_>::operator*=(long double lambda) {
    for(std::size_t i = 0; i < dim_; ++i) {
        coordinates_[i] *= lambda;
    }

    return *this;
}

//todo: avoid duplicate code(the same in point class)
template<std::size_t dim_>
bool lingeo::vector_t<dim_>::valid() const {
    for(const coordinate_t& elem : coordinates_)
        if(!lingeo::valid(elem))
            return false;
    return true;
}

template<std::size_t dim_>
long double lingeo::vector_t<dim_>::len() const {
    if(!valid()) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    return sqrt(dot(*this, *this));
}

template<std::size_t dim_>
bool lingeo::vector_t<dim_>::zero() const {
    if(!valid()) {
        return false;
    }


    for(std::size_t i = 0; i < dim_; ++i) {
        if(!equal(coordinates_[i], 0.0)) {
            return false;
        }
    }

    return true;
}

template<std::size_t dim_>
const lingeo::vector_t<dim_> lingeo::vector_t<dim_>::operator-() {
    lingeo::vector_t<dim_> ret;
    for(std::size_t i = 0; i < dim_; ++i) {
        ret[i] = -coordinates_[i];
    }

    return ret;
}

template<std::size_t dim_>
lingeo::coordinate_t &lingeo::vector_t<dim_>::operator[](std::size_t idx) {
    return coordinates_[idx];
}

template<std::size_t dim_>
const lingeo::coordinate_t &lingeo::vector_t<dim_>::operator[](std::size_t idx) const {
    return coordinates_[idx];
}

template<std::size_t dim_>
lingeo::vector_t<dim_>::vector_t(const lingeo::point_t<dim_> &point) {
    for(std::size_t i = 0; i < dim_; ++i) {
        coordinates_[i] = point[i];
    }
}

template<std::size_t dim_>
void lingeo::vector_t<dim_>::normalize() {
    if(!valid()) {
        return;
    }

    long double length = len();
    for(std::size_t i = 0; i < dim_; ++i) {
        coordinates_[i] /= length;
    }
}

//todo: for vectors with different dimensions
template<std::size_t DIM_>
long double lingeo::dot(const lingeo::vector_t<DIM_>& lhs, const lingeo::vector_t<DIM_>& rhs) {
    if(!lhs.valid() || !rhs.valid()) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    long double ans = 0;
    for(std::size_t i = 0; i < DIM_; ++i) {
        ans += lhs[i] * rhs[i];
    }

    return equal(ans, 0.0) ? 0.0 : ans;
}

template<std::size_t DIM_>
bool lingeo::operator==(const lingeo::vector_t<DIM_>& lhs, const lingeo::vector_t<DIM_>& rhs) {
    if(!lhs.valid() || !rhs.valid()) {
        return false;
    }

    for(std::size_t i = 0; i < DIM_; ++i) {
        if(!equal(lhs[i], rhs[i])) {
            return false;
        }
    }

    return true;
}

template<std::size_t DIM_>
lingeo::vector_t<DIM_> lingeo::operator+(const lingeo::vector_t<DIM_>& lhs, const lingeo::vector_t<DIM_>& rhs) {
    lingeo::vector_t<DIM_> tmp = lhs;
    return tmp += rhs;
}

template<std::size_t DIM_>
lingeo::vector_t<DIM_> lingeo::operator-(const lingeo::vector_t<DIM_>& lhs, const lingeo::vector_t<DIM_>& rhs) {
    lingeo::vector_t<DIM_> tmp = lhs;
    return tmp -= rhs;
}

template<std::size_t DIM_>
lingeo::vector_t<DIM_> lingeo::operator*(long double lambda, const lingeo::vector_t<DIM_>& rhs) {
    lingeo::vector_t<DIM_> tmp = rhs;
    return tmp *= lambda;
}

template<std::size_t DIM_>
lingeo::vector_t<DIM_> lingeo::operator*(const lingeo::vector_t<DIM_>& lhs, long double lambda) {
    return lambda * lhs;
}

template<std::size_t DIM_>
bool lingeo::parallel(const lingeo::vector_t<DIM_>& lhs, const lingeo::vector_t<DIM_>& rhs) {
    if(!lhs.valid() || !rhs.valid()) {
        return false;
    }

    long double dot_ = std::abs(dot(lhs, rhs));
    long double mult = lhs.len() * rhs.len();
    return equal(dot_, mult);
}

template<std::size_t DIM_>
std::istream& operator>>(std::istream& in, lingeo::vector_t<DIM_>& vector) {
    for(std::size_t i = 0; i < DIM_; ++i) {
        if(!in.good()) {
            return in;
        }
        in >> vector[i];
    }

    return in;
}

/*
 * todo: why it conflict with gtest?
template<std::size_t DIM_>
std::ostream& operator<<(std::ostream& out, const lingeo::vector_t<DIM_>& vector) {
    out << '(';
    out << vector[0];
    for(std::size_t i = 1; i < DIM_; ++i) {
        out << ", ";
        out << vector[i];
    }
    out << ')';
    return out;
}
*/