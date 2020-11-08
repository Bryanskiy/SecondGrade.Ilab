#pragma once

#include <utility>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <optional>
#include <iomanip>

#ifdef DEBUG_
#include <fstream>
#include <ios>
#include <string>

namespace debug {
std::ofstream log_file("log.txt");
std::string indent = "";

void increment_indent() {
    debug::indent += "        ";
}

void decrement_indent() {
    std::size_t indent_len = debug::indent.length();
    debug::indent.resize(indent_len - 8);
}

void print_block_separator() {
    log_file << "---------------------------------------------------------------------------------------" << std::endl;
}

template<typename T>
void print_matrix(T* data_, std::size_t rows, std::size_t cols) {
    for(std::size_t i = 0; i < rows; ++i) {
        debug::log_file << indent;
        for(std::size_t j = 0; j < cols; ++j) {
            debug::log_file << std::setw(8) << std::setprecision(5) << data_[j + i * rows] << ' ';
        }

        debug::log_file << std::endl;
    }
}
}
#endif

namespace matrix {
    const long double tolerance = 1e-7;

    template<typename T>
    class matrix_t final {
    public:
        /* constructors */
        matrix_t();
        matrix_t(std::size_t rows, std::size_t cols, T val = T{});
        template<typename it> matrix_t(std::size_t rows, std::size_t cols, it start, it end);
        template<typename U> matrix_t(const matrix_t<U>& rhs);

        /* big 3 */
        matrix_t(const matrix_t& rhs);
        matrix_t& operator=(const matrix_t& rhs);
        ~matrix_t();

        /* selectors */
        std::size_t get_row_number() const;
        std::size_t get_col_number() const;
        std::size_t get_elem_number() const;
        bool equal(const matrix_t& rhs) const;
        bool quadratic() const;
        std::optional<T> trace() const;

        /* useful methods */
        matrix_t& negate() &;
        matrix_t& transpose() &;
        std::size_t max_abs_col_elem(std::size_t idx, std::size_t start, std::size_t end) const;
        bool swap_rows(std::size_t lhs_idx, std::size_t rhs_idx);
        bool swap_cols(std::size_t lhs_idx, std::size_t rhs_idx);
        T det() const;

        /* operators */
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

    private:
        /* methods */
        bool initialized() const;

        /* fields */
        struct matrix_size_t {
            std::size_t row_size_;
            std::size_t col_size_;
        };
        T* data_;
        matrix_size_t size_;
    };

    template<typename T>
    bool operator==(const matrix_t<T>& lhs, const matrix_t<T>& rhs);

    /* useful functions */
    template<typename T>
    std::ostream& operator<<(std::ostream& out,  const matrix::matrix_t<T>& m);

    template<typename T>
    std::istream& operator>>(std::istream& in,  matrix::matrix_t<T>& m);

    template<typename T>
    matrix_t<T> multiplication(const matrix_t<T>& lhs, const matrix_t<T>& rhs);
}


/* -------------------------------------------------
                     REALIZATION
 --------------------------------------------------*/

template<typename T>
matrix::matrix_t<T>::matrix_t() : data_(nullptr), size_({0lu, 0lu}) {
#ifdef DEBUG_
    debug::print_block_separator();
    debug::log_file << debug::indent << "start default constructor" << std::endl;
    debug::log_file << debug::indent << "end default constructor" << std::endl;
    debug::print_block_separator();
#endif
}


template <typename T>
matrix::matrix_t<T>::matrix_t(std::size_t rows, std::size_t cols, T val) {
#ifdef DEBUG_
    debug::print_block_separator();
    debug::log_file << debug::indent << "start constructor matrix_t(row, col, val)" << std::endl;
#endif
    size_ = {rows, cols};
    std::size_t size = rows * cols;
    data_ = new T[size];
    for(std::size_t i = 0; i < size; ++i) {
        data_[i] = val;
    }
#ifdef DEBUG_
    debug::log_file << debug::indent << "adress of generated matrix: [" << data_ << "]" << std::endl;
    debug::log_file << debug::indent << "generated matrix: " << std::endl;
    debug::print_matrix(data_, rows, cols);
    debug::log_file << debug::indent << "end constructor matrix_t(row, col, val)" << std::endl;
    debug::print_block_separator();
#endif
}

template <typename T>
template <typename it>
matrix::matrix_t<T>::matrix_t(std::size_t rows, std::size_t cols, it start, it end) {
#ifdef DEBUG_
    debug::print_block_separator();
    debug::log_file << debug::indent << "start iterators constructor" << std::endl;
#endif
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
#ifdef DEBUG_
    debug::log_file << debug::indent << "adress of generated matrix: [" << data_ << "]" << std::endl;
    debug::log_file << debug::indent << "generated matrix: " << std::endl;
    debug::print_matrix(data_, rows, cols);
    debug::log_file << debug::indent << "end iterators constructor" << std::endl;
    debug::print_block_separator();
#endif
}

template<typename T>
template<typename U>
matrix::matrix_t<T>::matrix_t(const matrix_t<U>& rhs) {
#ifdef DEBUG_
    debug::print_block_separator();
    debug::log_file << debug::indent << "start template 'copy' constructor" << std::endl;
#endif
    size_ = {rhs.get_row_number(), rhs.get_col_number()};
    std::size_t size = rhs.get_elem_number();

    data_ = new T[size];
    for(std::size_t i = 0, maxi = size_.row_size_; i < maxi; ++i) {
        for(std::size_t j = 0, maxj = size_.col_size_; j < maxj; ++j) {
            data_[j + i * maxj] = rhs[i][j];
        }
    }
#ifdef DEBUG_
    debug::log_file << debug::indent << "adress of generated matrix: [" << data_ << "]" << std::endl;
    debug::log_file << debug::indent << "generated matrix: " << std::endl;
    debug::print_matrix(data_, size_.row_size_, size_.col_size_);
    debug::log_file << debug::indent << "end template 'copy' constructor" << std::endl;
    debug::print_block_separator();
#endif
}

template <typename T>
matrix::matrix_t<T>::matrix_t(const matrix_t<T>& rhs) {
#ifdef DEBUG_
    debug::print_block_separator();
    debug::log_file << debug::indent << "start copy constructor" << std::endl;
#endif
    size_ = {rhs.get_row_number(), rhs.get_col_number()};
    std::size_t matrix_size = rhs.get_elem_number();

    data_ = new T[matrix_size];
    for(std::size_t i = 0; i < matrix_size; ++i) {
        data_[i] = rhs.data_[i];
    }
#ifdef DEBUG_
    debug::log_file << debug::indent << "adress of generated matrix: [" << data_ << "]" << std::endl;
    debug::log_file << debug::indent << "generated matrix: " << std::endl;
    debug::print_matrix(data_, size_.row_size_, size_.col_size_);
    debug::log_file << debug::indent << "end copy constructor" << std::endl;
    debug::print_block_separator();
#endif
}

template <typename T>
matrix::matrix_t<T>& matrix::matrix_t<T>::operator=(const matrix_t<T>& rhs) {
#ifdef DEBUG_
    debug::print_block_separator();
    debug::log_file << debug::indent << "start copy operator" << std::endl;
#endif
    if(data_ == rhs.data_) {
        return *this;
    }

    std::size_t matrix_size = size_.row_size_ * size_.col_size_;
    if((get_row_number() != rhs.get_row_number()) || (get_col_number() != rhs.get_col_number())){
        delete[] data_;

        size_ = {rhs.size_.row_size_, rhs.size_.col_size_};
        matrix_size = size_.row_size_ * size_.col_size_;
        data_ = new T[matrix_size];
    }

    for(std::size_t i = 0; i < matrix_size; ++i) {
        data_[i] = rhs.data_[i];
    }

#ifdef DEBUG_
    debug::log_file << debug::indent << "new matrix adress: [" << data_ << "]" << std::endl;
    debug::log_file << debug::indent << "copied matrix: " << std::endl;
    debug::print_matrix(data_, size_.row_size_, size_.col_size_);
    debug::log_file << debug::indent << "end copy operator" << std::endl;
    debug::print_block_separator();
#endif

    return *this;
}

template <typename T>
matrix::matrix_t<T>::~matrix_t() {
#ifdef DEBUG_
    debug::print_block_separator();
    debug::log_file << debug::indent << "start destructor" << std::endl;
    debug::log_file << debug::indent << "delete data from [" << data_ << "]" << std::endl;
#endif
    delete[] data_;
#ifdef DEBUG_
    debug::log_file << debug::indent << "end destructor" << std::endl;
    debug::print_block_separator();
#endif
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
    if(!rhs.initialized() || !initialized()) {
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

    return true;
}

template <typename T>
std::optional<T> matrix::matrix_t<T>::trace() const {
    if(!quadratic()) {
        return std::optional<T>{};
    }

    std::optional<T> ret = 0;
    for(std::size_t i = 0, max = get_row_number(); i < max; ++i) {
        ret.value() += data_[i + max * i];
    }

    return ret;
}

template<typename T>
bool matrix::matrix_t<T>::initialized() const {
    return data_ != nullptr;
}

template <typename T>
matrix::matrix_t<T>& matrix::matrix_t<T>::negate() & {
    for (std::size_t i = 0, max = get_elem_number(); i < max; ++i) {
        data_[i] = -data_[i];
    }

    return *this;
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
    return static_cast<const matrix_t<T>&>(*this)[idx];
}

template<typename T>
const typename matrix::matrix_t<T>::proxy_row_t matrix::matrix_t<T>::operator[](std::size_t idx) const {
    proxy_row_t ret(data_ + idx * get_col_number());
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

template<typename T>
bool matrix::matrix_t<T>::swap_cols(std::size_t lhs_idx, std::size_t rhs_idx) {
    if(lhs_idx == rhs_idx) {
        return true;
    }

    std::size_t max_col = get_col_number();
    if((lhs_idx >= max_col) || (rhs_idx >= max_col)) {
        return false;
    }

    for(std::size_t i = 0, max = get_row_number(); i < max; ++i) {
        T tmp = data_[lhs_idx + i * max_col];
        data_[lhs_idx + i * max_col] = data_[rhs_idx + i * max_col];
        data_[rhs_idx + i * max_col] = tmp;
    }

    return true;
}

template<typename T>
T matrix::matrix_t<T>::det() const {
#ifdef DEBUG_
    debug::print_block_separator();
    debug::log_file << debug::indent << "start executing determinant for matrix: " << std::endl;
    debug::print_matrix(data_, get_row_number(), get_col_number());
#endif

    if(!quadratic()) {
        return T{};
    }

    double ret{1};

#ifdef DEBUG_
    debug::increment_indent();
#endif
    matrix_t<double> copy = *this;
#ifdef DEBUG_
    debug::decrement_indent();
#endif

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
#ifdef DEBUG_
    debug::log_file << "executed determinant = " << ret << std::endl;
    debug::log_file << "end executing determinant" << std::endl;
    debug::print_block_separator();
#endif

    return ret;
}

template<typename T>
bool matrix::matrix_t<T>::quadratic() const {
    return (get_row_number() == get_col_number()) && (get_row_number() != 0lu);
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
bool matrix::operator==(const matrix_t<T>& lhs, const matrix_t<T>& rhs) {
    return lhs.equal(rhs);
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
                elem += lhs[i][k] * rhs[k][j];
            }

            ret[i][j] = elem;
        }

    }

    return ret;
}
