
#pragma once

#include <utility>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <optional>
#include <iomanip>

#include "matrix_buffer.hpp"

namespace matrix {

const long double tolerance = 1e-5;

template<typename T>
class matrix_t : private matrix_buff_t<T> {

    using matrix_buff_t<T>::array_;
    using matrix_buff_t<T>::size_;
    using matrix_buff_t<T>::capacity_;
    using matrix_buff_t<T>::matrix_size_;

    using matrix_buff_t<T>::resize;

public:

    matrix_t() : matrix_buff_t<T>(0u, 0u) {}
    matrix_t(const matrix_t& rhs);
    matrix_t(std::size_t rows, std::size_t cols, T val = T{});
    matrix_t& operator=(const matrix_t& rhs);
    ~matrix_t() = default;

    std::size_t get_row_number() const   {return matrix_size_.rows_;}
    std::size_t get_col_number() const   {return matrix_size_.cols_;}
    std::size_t get_elem_number() const  {return size_;}

    void swap_rows(std::size_t lhs_idx, std::size_t rhs_idx);
    void swap_cols(std::size_t lhs_idx, std::size_t rhs_idx);
    std::size_t max_abs_col_elem(std::size_t idx, std::size_t start, std::size_t end) const;
    T det() const;

    class proxy_row_t {
        public:
            proxy_row_t(T* pointer) : row_(pointer) {}
            T& operator[](std::size_t idx) {return row_[idx];}
            const T& operator[](std::size_t idx) const {return row_[idx];}
        private:
            T* row_;
    };
        
    proxy_row_t operator[](std::size_t idx);
    const proxy_row_t operator[](std::size_t idx) const;
};

template<typename T>
std::ostream& operator<<(std::ostream& out,  const matrix::matrix_t<T>& m);

template<typename T>
std::istream& operator>>(std::istream& in,  matrix::matrix_t<T>& m);

template<typename T>
matrix_t<T> multiplication(const matrix_t<T>& lhs, const matrix_t<T>& rhs);

/* ------------------------------------------------------------------
                        IMPLEMENTATION
---------------------------------------------------------------------*/

template<typename T>
matrix_t<T>::matrix_t(const matrix_t& rhs) : matrix_buff_t<T>(rhs.matrix_size_.rows_, rhs.matrix_size_.cols_) {
    while(size_ < rhs.size_) {
        copy_construct(array_ + size_, rhs.array_[size_]);
        ++size_;
    }
}

template<typename T>
matrix_t<T>::matrix_t(std::size_t rows, std::size_t cols, T val) : matrix_buff_t<T>(rows, cols) {
    std::size_t num_elems = rows * cols;
    while(size_ < num_elems) {
        copy_construct(array_ + size_, val);
        ++size_;
    }
}

template<typename T>
matrix_t<T>& matrix_t<T>::operator=(const matrix_t& rhs) {
    matrix_t tmp(rhs.matrix_size_.rows_, rhs.matrix_size_.cols_);
    swap_buffers(tmp);
    return *this;
}

template<typename T>
void matrix::matrix_t<T>::swap_rows(std::size_t lhs_idx, std::size_t rhs_idx) {
    if(lhs_idx == rhs_idx) {
        return;
    }

    for(std::size_t i = 0, max = get_col_number(); i < max; ++i) {
        T tmp = array_[i + lhs_idx * max];
        array_[i + lhs_idx * max] = array_[i + rhs_idx * max];
        array_[i + rhs_idx * max] = tmp;
    }

    return;
}

template<typename T>
void matrix::matrix_t<T>::swap_cols(std::size_t lhs_idx, std::size_t rhs_idx) {
    if(lhs_idx == rhs_idx) {
        return;
    }

    std::size_t max_col = get_col_number();

    for(std::size_t i = 0, max = get_row_number(); i < max; ++i) {
        T tmp = array_[lhs_idx + i * max_col];
        array_[lhs_idx + i * max_col] = array_[rhs_idx + i * max_col];
        array_[rhs_idx + i * max_col] = tmp;
    }

    return;
}

template<typename T>
std::size_t matrix::matrix_t<T>::max_abs_col_elem(std::size_t idx, std::size_t start, std::size_t end) const{
    std::size_t ret = start;

    std::size_t max_col = get_col_number();
    for(std::size_t i = start + 1; i < end; ++i) {
        if(std::abs(array_[idx + i * max_col]) > std::abs(array_[idx + ret * max_col])) {
            ret = i;
        }
    }

    return ret;
}

template<typename T>
T matrix::matrix_t<T>::det() const {
    long double ret = 1.0;

    matrix_t<long double> copy = *this;

    std::size_t current_m = 0;
    std::size_t current_n = 0;

    std::size_t max_m = copy.get_row_number();
    std::size_t max_n = copy.get_col_number();

    while((current_m < max_m) && (current_n < max_n)) {
        std::size_t max_elem = copy.max_abs_col_elem(current_n, current_m, max_m);
        if(copy[max_elem][current_n] == 0) {
            ++current_n;
            continue;
        }

        if(max_elem != current_m) {
            ret *= -1;
            copy.swap_rows(max_elem, current_m);
        }

        for(std::size_t i = current_m + 1; i < max_m; ++i) {
            double f = copy[i][current_n] / copy[current_m][current_n];
            copy[i][current_n] = 0.0;
            for (std::size_t j = current_n + 1; j < max_n; ++j) {
                copy[i][j] = copy[i][j] - f * copy[current_m][j];
            }
        }

        ++current_m;
        ++current_n;
    }

    for(std::size_t i = 0lu, max = copy.get_col_number(); i < max; ++i) {
        if(std::abs(ret) < tolerance) {
            return 0.0;
        }

        ret *= copy[i][i];
    }

    return ret;
}

template<typename T>
typename matrix::matrix_t<T>::proxy_row_t matrix::matrix_t<T>::operator[](std::size_t idx) {
    return static_cast<const matrix_t<T>&>(*this)[idx];
}

template<typename T>
const typename matrix::matrix_t<T>::proxy_row_t matrix::matrix_t<T>::operator[](std::size_t idx) const {
    proxy_row_t ret(array_ + idx * get_col_number());
    return ret;
}

template<typename T>
std::ostream& matrix::operator<<(std::ostream& out,  const matrix::matrix_t<T>& m) {
    if(m.get_elem_number() == 0) {
        return out;
    }

    for(std::size_t i = 0; i < m.get_row_number(); ++i) {
        for(std::size_t j = 0; j < m.get_col_number(); ++j) {
            out << std::setw(8) << std::setprecision(matrix::tolerance) << m[i][j] << ' ';
        }

        out << std::endl;
    }

    return out;
}

template <typename T>
std::istream& matrix::operator>>(std::istream& in,  matrix::matrix_t<T>& m) {
    if(m.get_elem_number() == 0) {
        return in;
    }

    for(std::size_t i = 0, maxi = m.get_col_number(); i < maxi; ++i) {
        for(std::size_t j = 0, maxj = m.get_row_number(); j < maxj; ++j) {
            in >> m[i][j];
        }
    }

    return in;
}

template<typename T>
matrix::matrix_t<T> matrix::multiplication(const matrix_t<T>& lhs, const matrix_t<T>& rhs) {
    if(lhs.get_col_number() != rhs.get_row_number()) {
        return matrix_t<T>{};
    }

    std::size_t ret_m = lhs.get_row_number();
    std::size_t ret_n = rhs.get_col_number();

    matrix_t<T> ret{ret_m, ret_n};
    for(std::size_t i = 0, maxi = ret_m; i < maxi; ++i) {
        for(std::size_t j = 0, maxj = ret_n; j < maxj; ++j) {
            T elem{};
            for(std::size_t k = 0, maxk = lhs.get_col_number(); k < maxk; ++k) {
                elem += (lhs[i][k] * rhs[k][j]);
            }

            ret[i][j] = elem;
        }

    }

    return ret;
}

} /* namespace matrix */