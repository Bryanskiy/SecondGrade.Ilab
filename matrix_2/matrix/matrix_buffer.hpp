#pragma once

namespace matrix {

template<typename T>
void copy_construct(T* p, const T& val) {
    new (p) T(val);
}

template<typename T>
void destroy(T* p) {
    p->~T();
}

template <typename FwdIter>
void destroy(FwdIter first, FwdIter last) {  
    while (first != last) { 
        destroy(&*first++);
    }     
}

template<typename T>
class matrix_buff_t {

public:    
    matrix_buff_t(const matrix_buff_t&) = delete;
    matrix_buff_t& operator=(const matrix_buff_t&) = delete; 

protected:

    T* array_;
    std::size_t size_;
    std::size_t capacity_;
    struct matrix_size_t {
        std::size_t rows_;
        std::size_t cols_;
    } matrix_size_;

/* what happend if constructor throw exeption ? */
    matrix_buff_t(std::size_t rows, std::size_t cols) : 
        array_{(0 == cols * rows) ? nullptr : static_cast<T*>(::operator new(sizeof(T) * rows * cols)) },
        size_{0u},
        capacity_{rows * cols},
        matrix_size_{rows, cols} {}

    ~matrix_buff_t() {
        destroy(array_, array_ + size_);
        ::operator delete(array_);
    }

    void swap_buffers(matrix_buff_t& rhs) noexcept {
        std::swap(array_, rhs.array_);
        std::swap(size_, rhs.size_);
        std::swap(capacity_, rhs.capacity_);
        std::swap(matrix_size_, rhs.matrix_size_);
    }

    void resize(std::size_t rows, std::size_t cols);

}; /* matrix_buff_t */

template<typename T>
void matrix_buff_t<T>::resize(std::size_t rows, std::size_t cols) {
    matrix_buff_t<T> tmp(rows, cols);

    for(std::size_t i = 0, maxi = rows; i < maxi; ++i) {
        for(std::size_t j = 0, maxj = cols; j < maxj; ++j) {
            T val();
            if((j > matrix_size_.rows_) && (i < matrix_size_.cols_)) {
               val = array_[j + i * rows]; 
            }

            copy_construct(tmp + j + i * rows, val);
        }
    }
}

} /* namespace matrix */