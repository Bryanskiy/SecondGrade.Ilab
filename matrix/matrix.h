#pragma once

#include <utility>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <iostream>

namespace matrix {

    template<typename T>
    class matrix_t {
    public:
        /* constructors */
        matrix_t();
        matrix_t(std::size_t rows, std::size_t cols, T val = T{});
        template<typename it> matrix_t(std::size_t rows, std::size_t cols, it start, it end);

        /* big 3 */
        matrix_t(const matrix_t<T>& rhs);
        matrix_t& operator=(const matrix_t<T>& rhs);
        ~matrix_t();

        /* selectors */
        std::size_t get_row_number() const;
        std::size_t get_col_number() const;
        std::size_t get_elem_number() const;
        bool equal(const matrix_t<T>& rhs) const;
        T trace() const;

        /* useful methods */
        bool valid() const;
        matrix_t& negate() &;
        matrix_t& transpose() &;
        std::size_t max_abs_col_elem(std::size_t idx, std::size_t start, std::size_t end) const;
        bool swap_rows(std::size_t lhs_idx, std::size_t rhs_idx);

        /* operators */
        class proxy_row_t {
            public:
                void set_pointer(T* pointer) {row_ = pointer;}
                T& operator[](std::size_t idx) {return row_[idx];}
                const T& operator[](std::size_t idx) const {return row_[idx];}
            private:
                T* row_;
        };
        proxy_row_t operator[](std::size_t idx);
        const proxy_row_t operator[](std::size_t idx) const;

    private:
        struct matrix_size_t {
            std::size_t row_size_;
            std::size_t col_size_;
        };
        T* data_;
        matrix_size_t size_;
    };

    template<typename T>
    bool operator==(const matrix_t<T>& lhs, const matrix_t<T>& rhs);

    template <typename T>
    std::istream& operator>>(std::istream& in,  matrix::matrix_t<T>& m);

    double det(const matrix_t<double>& m);
}


/* -------------------------------------------------
                     REALIZATION
 --------------------------------------------------*/

template <typename T>
matrix::matrix_t<T>::matrix_t() {
    data_ = nullptr;
    size_ = {0lu, 0lu};
}

template <typename T>
matrix::matrix_t<T>::matrix_t(std::size_t rows, std::size_t cols, T val) {
    size_ = {rows, cols};
    std::size_t size = rows * cols;
    data_ = new T[size];
    for(std::size_t i = 0; i < size; ++i) {
        data_[i] = val;
    }
}

template <typename T>
template <typename it>
matrix::matrix_t<T>::matrix_t(std::size_t rows, std::size_t cols, it start, it end) {
    size_ = {rows, cols};
    std::size_t size = rows * cols;
    data_ = new T[size];
    for(std::size_t i = 0; i < size;  ++i) {
        if(start == end) {
            break;
        }
        data_[i] = *start;
        ++start;
    }
}

template <typename T>
matrix::matrix_t<T>::matrix_t(const matrix_t<T>& rhs) {
    if(data_ == rhs.data_) {
        return;
    }

    size_ = {rhs.size_.row_size_, rhs.size_.col_size_};
    std::size_t matrix_size = size_.row_size_ * size_.col_size_;

    data_ = new T[matrix_size];
    for(std::size_t i = 0; i < matrix_size; ++i) {
        data_[i] = rhs.data_[i];
    }
}

template <typename T>
matrix::matrix_t<T>& matrix::matrix_t<T>::operator=(const matrix_t<T>& rhs) {
    if(data_ == rhs.data_) {
        return *this;
    }

    if(data_ != nullptr) {
        delete[] data_;
    }

    size_ = {rhs.size_.row_size_, rhs.size_.col_size_};
    std::size_t matrix_size = size_.row_size_ * size_.col_size_;

    data_ = new T[matrix_size];
    for(std::size_t i = 0; i < matrix_size; ++i) {
        data_[i] = rhs.data_[i];
    }

    return *this;
}

template <typename T>
matrix::matrix_t<T>::~matrix_t() {
    if(valid()) {
        delete[] data_;
    }
}

template <typename T>
std::size_t matrix::matrix_t<T>::get_row_number() const {
    return size_.row_size_;
}

template <typename T>
std::size_t matrix::matrix_t<T>::get_col_number() const {
    return size_.col_size_;
}

template <typename T>
std::size_t matrix::matrix_t<T>::get_elem_number() const {
    return get_row_number() * get_col_number();
}

template <typename T>
bool matrix::matrix_t<T>::equal(const matrix_t<T>& rhs) const {
    if(!rhs.valid()) {
        return false;
    }

    if((get_col_number() != rhs.get_col_number()) && (get_row_number() != rhs.get_row_number())) {
        return false;
    }

    for(std::size_t i = 0, max = get_elem_number(); i < max; ++i) {
        if(data_[i] != rhs.data_[i]) {
            return false;
        }
    }
}

template <typename T>
T matrix::matrix_t<T>::trace() const {
    if(get_col_number() != get_row_number()) {
        return T{};
    }

    T ans{};
    for(std::size_t i = 0, max = get_row_number(); i < max; ++i) {
        ans += data_[i + max * i];
    }

    return ans;
}

template<typename T>
bool matrix::matrix_t<T>::valid() const {
    if(data_ == nullptr) {
        return false;
    }

    return true;
}

template <typename T>
matrix::matrix_t<T>& matrix::matrix_t<T>::negate() & {
    for(std::size_t i = 0, max = get_elem_number(); i < max; ++i) {
        data_[i] = -data_[i];
    }
}

template <typename T>
matrix::matrix_t<T>& matrix::matrix_t<T>::transpose() & {
    matrix_t<T> tmp_matrix = *this;

    std::size_t tmp_value = size_.row_size_;
    size_.row_size_ = size_.col_size_;
    size_.col_size_ = tmp_value;

    for(std::size_t i = 0, maxi = get_row_number(); i < maxi; ++i) {
        for(std::size_t j = 0, maxj = get_col_number(); j < maxj; ++j) {
            data_[j + i * maxj] = tmp_matrix[j][i];
        }
    }
}

template<typename T>
typename matrix::matrix_t<T>::proxy_row_t matrix::matrix_t<T>::operator[](std::size_t idx) {
    proxy_row_t ret;
    ret.set_pointer(data_ + idx * get_col_number());
    return ret;
}

template<typename T>
const typename matrix::matrix_t<T>::proxy_row_t matrix::matrix_t<T>::operator[](std::size_t idx) const {
    proxy_row_t ret;
    ret.set_pointer(data_ + idx * get_col_number());
    return ret;
}

template<typename T>
std::size_t matrix::matrix_t<T>::max_abs_col_elem(std::size_t idx, std::size_t start, std::size_t end) const{
    std::size_t ret = start;

    std::size_t max_col = get_col_number();
    for(std::size_t i = start + 1; i < end; ++i) {
        if(std::abs(data_[idx + i * max_col]) > std::abs(data_[idx + ret * max_col])) {
            ret = i;
        }
    }

    return ret;
}

template<typename T>
bool matrix::matrix_t<T>::swap_rows(std::size_t lhs_idx, std::size_t rhs_idx) {
    if(lhs_idx == rhs_idx) {
        return true;
    }

    std::size_t max_row = get_row_number();
    if((lhs_idx >= max_row) || (rhs_idx >= max_row)) {
        return false;
    }

    for(std::size_t i = 0, max = get_col_number(); i < max; ++i) {
        T tmp = data_[i + lhs_idx * max];
        data_[i + lhs_idx * max] = data_[i + rhs_idx * max];
        data_[i + rhs_idx * max] = tmp;
    }

    return true;
}

template <typename T>
std::istream& matrix::operator>>(std::istream& in,  matrix::matrix_t<T>& m) {
    if(!m.valid()) {
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
bool matrix::operator==(const matrix_t<T>& lhs, const matrix_t<T>& rhs) {
    if(!lhs.valid()) {
        return false;
    }

    return lhs.equal(rhs);
}

double matrix::det(const matrix_t<double>& m) {
    double ret{1};

    matrix_t<double> copy = m;

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

    for(std::size_t i = 0lu, max = copy.get_col_number();i < max; ++i) {
        if(std::abs(ret) < 1e-7) {
            return 0.0;
        }

        ret *= copy[i][i];
    }

    return ret;
}



