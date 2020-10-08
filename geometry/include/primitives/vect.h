#pragma once

#include "general.h"

namespace ivkg {

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
ivkg::vector_t<dim_>::vector_t() {
    for(coordinate_t& elem : coordinates_) {
        elem = std::numeric_limits<double>::quiet_NaN();
    }
}


//todo: avoid duplicate code (the same constructor in point class)
template<std::size_t dim_>
ivkg::vector_t<dim_>::vector_t(std::initializer_list<coordinate_t> coordinates) {
    if(coordinates.size() != dim_) {
        return;
    }

    typename std::array<coordinate_t, dim_>::iterator obj_iter  = coordinates_.begin();
    typename std::initializer_list<coordinate_t>::iterator init_iter = coordinates.begin();
    while (obj_iter != coordinates_.end()) {
        *obj_iter = *init_iter;
        obj_iter++;
        init_iter++;
    }
}

//todo: construct vector from points with different dimensions
template<std::size_t dim_>
ivkg::vector_t<dim_>::vector_t(const point_t<dim_>& lhs, const point_t<dim_>& rhs) {
    for(std::size_t i = 0; i < dim_; ++i) {
        coordinates_[i] = rhs[i] - lhs[i];
    }
}

template<std::size_t dim_>
ivkg::vector_t<dim_>& ivkg::vector_t<dim_>::operator+=(const vector_t<dim_>& rhs) {
    for(std::size_t i = 0; i < dim_; ++i) {
        this->coordinates_[i] += rhs[i];
    }

    return *this;
}

//todo: avoid duplicate code (the same in operator +=)
template<std::size_t dim_>
ivkg::vector_t<dim_>& ivkg::vector_t<dim_>::operator-=(const vector_t<dim_>& rhs) {
    for(std::size_t i = 0; i < dim_; ++i) {
        this->coordinates_[i] -= rhs[i];
    }

    return *this;
}

//todo: overflow protection
template<std::size_t dim_>
ivkg::vector_t<dim_>& ivkg::vector_t<dim_>::operator*=(long double lambda) {
    for(std::size_t i = 0; i < dim_; ++i) {
        this->coordinates_[i] *= lambda;
    }

    return *this;
}

//todo: avoid duplicate code(the same in point class)
template<std::size_t dim_>
bool ivkg::vector_t<dim_>::valid() const {
    bool flag = true;
    for(const coordinate_t& elem : coordinates_) {
        flag = flag && ivkg::valid(elem);
    }

    return flag;
}

template<std::size_t dim_>
long double ivkg::vector_t<dim_>::len() const {
    return sqrt(dot(*this, *this));
}

template<std::size_t dim_>
bool ivkg::vector_t<dim_>::zero() const {
    if(!this->valid()) {
        return false;
    }


    for(std::size_t i = 0; i < dim_; ++i) {
        if(!equal(this->coordinates_[i], 0.0)) {
            return false;
        }
    }

    return true;
}

template<std::size_t dim_>
const ivkg::vector_t<dim_> ivkg::vector_t<dim_>::operator-() {
    ivkg::vector_t<dim_> ret;
    for(std::size_t i = 0; i < dim_; ++i) {
        ret[i] = -coordinates_[i];
    }

    return ret;
}

template<std::size_t dim_>
ivkg::coordinate_t &ivkg::vector_t<dim_>::operator[](std::size_t idx) {
    return coordinates_[idx];
}

template<std::size_t dim_>
const ivkg::coordinate_t &ivkg::vector_t<dim_>::operator[](std::size_t idx) const {
    return coordinates_[idx];
}

template<std::size_t dim_>
ivkg::vector_t<dim_>::vector_t(const ivkg::point_t<dim_> &point) {
    for(std::size_t i = 0; i < dim_; ++i) {
        coordinates_[i] = point[i];
    }
}

template<std::size_t dim_>
void ivkg::vector_t<dim_>::normalize() {
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
long double ivkg::dot(const ivkg::vector_t<DIM_>& lhs, const ivkg::vector_t<DIM_>& rhs) {
    if(!lhs.valid() || !rhs.valid()) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    long double ans = 0;
    for(std::size_t i = 0; i < DIM_; ++i) {
        ans += lhs[i] * rhs[i];
    }

    return ans;
}

//todo: for all dimensions
ivkg::vector_t<3> ivkg::cross(const ivkg::vector_t<3>& lhs, const ivkg::vector_t<3>& rhs) {
    ivkg::coordinate_t x = lhs[1] * rhs[2] - lhs[2] * rhs[1];
    ivkg::coordinate_t y = lhs[2] * rhs[0] - lhs[0] * rhs[2];
    ivkg::coordinate_t z = lhs[0] * rhs[1] - lhs[1] * rhs[0];
    return ivkg::vector_t<3>({x, y, z});
}

template<std::size_t DIM_>
bool ivkg::operator==(const ivkg::vector_t<DIM_>& lhs, const ivkg::vector_t<DIM_>& rhs) {
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
ivkg::vector_t<DIM_> ivkg::operator+(const ivkg::vector_t<DIM_>& lhs, const ivkg::vector_t<DIM_>& rhs) {
    ivkg::vector_t<DIM_> tmp = lhs;
    return tmp += rhs;
}

template<std::size_t DIM_>
ivkg::vector_t<DIM_> ivkg::operator-(const ivkg::vector_t<DIM_>& lhs, const ivkg::vector_t<DIM_>& rhs) {
    ivkg::vector_t<DIM_> tmp = lhs;
    return tmp -= rhs;
}

template<std::size_t DIM_>
ivkg::vector_t<DIM_> ivkg::operator*(long double lambda, const ivkg::vector_t<DIM_>& rhs) {
    ivkg::vector_t<DIM_> tmp = rhs;
    return tmp *= lambda;
}

template<std::size_t DIM_>
ivkg::vector_t<DIM_> ivkg::operator*(const ivkg::vector_t<DIM_>& lhs, long double lambda) {
    return lambda * lhs;
}

template<std::size_t DIM_>
bool ivkg::parallel(const ivkg::vector_t<DIM_>& lhs, const ivkg::vector_t<DIM_>& rhs) {
    if(!lhs.valid() || !rhs.valid()) {
        return false;
    }

    return equal(dot(lhs, rhs), lhs.len() * rhs.len());
}

template<std::size_t DIM_>
std::istream& operator>>(std::istream& in, ivkg::vector_t<DIM_>& vector) {
    for(std::size_t i = 0; i < DIM_; ++i) {
        in >> vector[i];
    }

    return in;
}

/*
 * todo: why it conflict with gtest?
template<std::size_t DIM_>
std::ostream& operator<<(std::ostream& out, const ivkg::vector_t<DIM_>& vector) {
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