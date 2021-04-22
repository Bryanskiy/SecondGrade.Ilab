#pragma once

#include <vector>

#include "cl_general.hpp"


namespace clf {

class cl_fvector_t final : public cl_iclass_t {
public:

    cl_fvector_t(const std::vector<float>& data) : data_(data) {}
    cl_fvector_t(std::vector<float>&& data) : data_(data) {}

    std::size_t size() const { return data_.size(); }

    cl_fvector_t& negate() &;
    cl_fvector_t& scalar_mult(float value);
    cl_fvector_t& operator+=(const cl_fvector_t& rhs);
    cl_fvector_t& operator-=(const cl_fvector_t& rhs);
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

} /* namespace clf */