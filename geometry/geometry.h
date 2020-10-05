#pragma once

#include <iostream>
#include <limits>
#include <cmath>
#include <array>

namespace ivkg {
    const long double tolerance = 1e-5;

    using coordinate_t = long double;

    bool valid(const coordinate_t& coordinate) {
        return !(std::isnan(coordinate) || !std::isfinite(coordinate));
    }

    bool equal(const coordinate_t& lhs, const coordinate_t& rhs) {
        return valid(lhs) && valid(rhs) && (std::abs(lhs - rhs) < tolerance);
    }
}

namespace ivkg {

    /* ------------------------------------------------
                      POINT_CLASS
    -------------------------------------------------*/
    template<std::size_t dim_>
    class point_t {

    public:
        point_t();
        point_t(const point_t<dim_> &) = default;
        point_t(std::initializer_list <coordinate_t> coordinates);
        ~point_t() = default;

        bool valid() const;
        bool operator==(const point_t<dim_>& rhs) const;

        coordinate_t &operator[](std::size_t idx);
        const coordinate_t &operator[](std::size_t idx) const;

        long double distance() const;

    private:
        std::array <coordinate_t, dim_> coordinates_;
    };

    template<std::size_t DIM_>
    std::istream &operator>>(std::istream &in, point_t<DIM_> &point);

    template<std::size_t DIM_>
    std::ostream &operator<<(std::ostream &out, const point_t<DIM_> &point);


    /* ------------------------------------------------
                      VECTOR_CLASS
    -------------------------------------------------*/
    template<std::size_t dim_>
    class vector_t {
        public:
            vector_t();
            vector_t(const vector_t<dim_> &) = default;
            vector_t(std::initializer_list<coordinate_t> coordinates);
            vector_t(const point_t<dim_>& lhs, const point_t<dim_>& rhs);
            vector_t(const point_t<dim_>& point);
            ~vector_t() = default;

            coordinate_t &operator[](std::size_t idx);
            const coordinate_t &operator[](std::size_t idx) const;

            vector_t<dim_>& operator+=(const vector_t<dim_>& rhs);
            vector_t<dim_>& operator-=(const vector_t<dim_>& rhs);
            vector_t<dim_>& operator*=(long double lambda);
            const vector_t<dim_> operator-();

            bool valid() const;
            long double len() const;
            bool zero() const;
        private:
            std::array <coordinate_t, dim_> coordinates_;
    };

    template<std::size_t DIM_>
    long double dot(const vector_t<DIM_>& lhs, const vector_t<DIM_>& rhs);

    vector_t<3> cross(const vector_t<3>& lhs, const vector_t<3>& rhs);

    template<std::size_t DIM_>
    bool operator==(const vector_t<DIM_>& lhs, const vector_t<DIM_>& rhs);

    template<std::size_t DIM_>
    vector_t<DIM_> operator+(const vector_t<DIM_>& lhs, const vector_t<DIM_>& rhs);

    template<std::size_t DIM_>
    vector_t<DIM_> operator-(const vector_t<DIM_>& lhs, const vector_t<DIM_>& rhs);

    template<std::size_t DIM_>
    vector_t<DIM_> operator*(long double lambda, const vector_t<DIM_>& rhs);

    template<std::size_t DIM_>
    vector_t<DIM_> operator*(const vector_t<DIM_>& lhs, long double lambda);

    template<std::size_t DIM_>
    bool parallel(const vector_t<DIM_>& lhs, const vector_t<DIM_>& rhs);

    template<std::size_t DIM_>
    std::istream& operator>>(std::istream& in, vector_t<DIM_>& vector);

    /*
    template<std::size_t DIM_>
    std::ostream& operator<<(std::ostream& out, const vector_t<DIM_>& vector);
    */

    /* ------------------------------------------------
                      LINE_CLASS
    -------------------------------------------------*/
    template<std::size_t dim_>
    class line_t {
        public:
            line_t() = default;
            line_t(const line_t<dim_>&) = default;
            line_t(const vector_t<dim_>& lhs, const vector_t<dim_>& rhs);
            line_t(const point_t<dim_>& lhs, const point_t<dim_>& rhs);

            vector_t<dim_> get_direction() const;
            vector_t<dim_> get_start() const;

            bool valid() const;
        private:
            vector_t<dim_> start_;
            vector_t<dim_> direction_;
    };

    template<std::size_t DIM_>
    bool operator==(const line_t<DIM_>& lhs, const line_t<DIM_>& rhs);

    template<std::size_t DIM_>
    bool parallel(const line_t<DIM_>& lhs, const line_t<DIM_>& rhs);

    /* ------------------------------------------------
                      PLANE_CLASS
    -------------------------------------------------*/
    class plane_t {
        public:
            plane_t();
            plane_t(const plane_t &) = default;
            plane_t(std::initializer_list<long double> coefficients);

            long double& operator[](std::size_t idx);
            const long double& operator[](std::size_t idx) const;

            vector_t<3> normal() const;
            line_t<3> intersect(const plane_t& rhs) const;
        private:
            std::array<long double, 4> coefficients_;
    };

    bool operator==(const plane_t& lhs, const plane_t& rhs);
    bool parallel(const plane_t& lhs, const plane_t& rhs);
}


/* ------------------------------------------------
                REALIZATION_POINT_CLASS
-------------------------------------------------*/

template<std::size_t DIM_>
std::istream& ivkg::operator>>(std::istream& in, ivkg::point_t<DIM_>& point) {
    for(std::size_t i = 0; i < DIM_; ++i) {
        in >> point[i];
    }

    return in;
}

template<std::size_t DIM_>
std::ostream& ivkg::operator<<(std::ostream& out, const ivkg::point_t<DIM_>& point) {
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
ivkg::point_t<dim_>::point_t() {
    for(coordinate_t& elem : coordinates_) {
        elem = std::numeric_limits<double>::quiet_NaN();
    }
}

template<std::size_t dim_>
ivkg::point_t<dim_>::point_t(std::initializer_list<coordinate_t> coordinates) {
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

template<std::size_t dim_>
bool ivkg::point_t<dim_>::valid() const {
    bool flag = true;
    for(const coordinate_t& elem : coordinates_) {
        flag = flag && ivkg::valid(elem);
    }

    return flag;
}
template<std::size_t dim_>
bool ivkg::point_t<dim_>::operator==(const point_t<dim_>& rhs) const {
    bool flag = true;

    typename std::array<coordinate_t, dim_>::const_iterator rhs_iter = rhs.coordinates_.begin();
    for(const auto& lhs_coordinate : coordinates_) {
        flag = flag && equal(lhs_coordinate, *rhs_iter);
        ++rhs_iter;
    }

    return flag;
}

template<std::size_t dim_>
ivkg::coordinate_t& ivkg::point_t<dim_>::operator[](std::size_t idx) {
    return coordinates_[idx];
}

template<std::size_t dim_>
const ivkg::coordinate_t& ivkg::point_t<dim_>::operator[](std::size_t idx) const {
    return coordinates_[idx];
}

template<std::size_t dim_>
long double ivkg::point_t<dim_>::distance() const {
    return ivkg::vector_t<dim_>(coordinates_).len();
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

/* ------------------------------------------------
                PLANE_LINE_CLASS
-------------------------------------------------*/

ivkg::plane_t::plane_t() {
    for(coordinate_t& elem : coefficients_) {
        elem = std::numeric_limits<double>::quiet_NaN();
    }
}

ivkg::plane_t::plane_t(std::initializer_list<long double> coefficients) {
    if(coefficients.size() != coefficients_.size()) {
        return;
    }

    typename std::array<coordinate_t, 4>::iterator obj_iter  = coefficients_.begin();
    typename std::initializer_list<coordinate_t>::iterator init_iter = coefficients.begin();
    while (obj_iter != coefficients_.end()) {
        *obj_iter = *init_iter;
        obj_iter++;
        init_iter++;
    }
}

long double& ivkg::plane_t::operator[](std::size_t idx) {
    return coefficients_[idx];
}

const long double& ivkg::plane_t::operator[](std::size_t idx) const {
    return coefficients_[idx];
}

ivkg::vector_t<3> ivkg::plane_t::normal() const {
    return ivkg::vector_t<3>({coefficients_[0], coefficients_[1], coefficients_[3]});
}

ivkg::line_t<3> ivkg::plane_t::intersect(const ivkg::plane_t &rhs) const {
    vector_t<3> lhs_normal = normal();
    vector_t<3> rhs_normal = rhs.normal();
    vector_t<3> crs = cross(lhs_normal, rhs_normal);
    if(equal(crs.len(), 0.0)) {
        return ivkg::line_t<3>();
    }

    long double s1, s2, a, b;
    s1 = coefficients_[3];
    s2 = rhs[3];

    long double n1n2dot = dot(lhs_normal, rhs_normal);
    long double n1sqr = dot(lhs_normal, lhs_normal);
    long double n2sqr = dot(rhs_normal, rhs_normal);

    long double denominator = -std::pow(n1n2dot, 2) + n1sqr * n2sqr;
    a = (s2 * n1n2dot - s1 * n2sqr) / denominator;
    b = (s1 * n1n2dot - s2 * n1sqr) / denominator;

    return line_t<3>(a * lhs_normal + b * rhs_normal, crs);
}