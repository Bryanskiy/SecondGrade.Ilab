#pragma once

#include <vector>

#include "cl_general.hpp"
#include "../support/matrix/matrix.hpp"


namespace clf {

/*-------------------------------------------------------------------------
                                cl_fvector_t
--------------------------------------------------------------------------*/

class cl_fvector_t final : public cl_iclass_t {
public:

    cl_fvector_t(std::size_t size) : data_(size) {}
    cl_fvector_t(const std::vector<float>& data) : data_(data) {}
    cl_fvector_t(std::vector<float>&& data) : data_(data) {}

    std::size_t size() const { return data_.size(); }

    cl_fvector_t& negate() &;
    cl_fvector_t& scalar_mult(float value);
    cl_fvector_t& scalar_mult(cl_fvector_t& value);
    cl_fvector_t& operator+=(const cl_fvector_t& rhs);
    cl_fvector_t& operator-=(const cl_fvector_t& rhs);
    cl_fvector_t& operator*=(const cl_fvector_t& rhs);
    float& operator[](std::size_t idx) {return data_[idx];}
    bool operator<(const cl_fvector_t& lhs);
    bool operator>(const cl_fvector_t& lhs);
    bool operator==(const cl_fvector_t& lhs);
    bool operator!=(const cl_fvector_t& lhs);


    ~cl_fvector_t() = default;
private:
    std::vector<float> data_;
};

const cl_fvector_t operator+(const cl_fvector_t& rhs, const cl_fvector_t& lhs);
const cl_fvector_t operator-(const cl_fvector_t& rhs, const cl_fvector_t& lhs);


/*-------------------------------------------------------------------------
                                cl_imatrix_t
--------------------------------------------------------------------------*/

class cl_imatrix_t : public cl_iclass_t {
public:

    std::size_t cols() const {return cols_;}
    std::size_t rows() const {return rows_;}

    virtual ~cl_imatrix_t() = 0;

protected:
    std::size_t cols_;
    std::size_t rows_;
};

/*-------------------------------------------------------------------------
                 cl_bandet_sparce_matrix_t REALIZATION
--------------------------------------------------------------------------*/

class cl_bandet_sparce_fmatrix_t final : public cl_imatrix_t {
public:

    cl_bandet_sparce_fmatrix_t(matrix::matrix_t<float>& matrix);

    cl_fvector_t vector_mult(cl_fvector_t& rhs);

    ~cl_bandet_sparce_fmatrix_t() = default;    

private:
    std::vector<cl_fvector_t> diagonals_;
    std::vector<std::size_t> indices_;
};

} /* namespace clf */