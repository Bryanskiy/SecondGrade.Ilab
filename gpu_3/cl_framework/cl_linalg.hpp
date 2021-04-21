#pragma once

#include <vector>

#include "cl_general.hpp"


namespace clf {

class cl_fvector_t final : public cl_iclass_t {
public:

    std::size_t size() const { return data_.size(); }

    cl_fvector_t& operator+=(const cl_fvector_t& rhs);
    float& operator[](std::size_t idx) {return data_[idx];}
private:
    std::vector<float> data_;
};

const cl_fvector_t operator+(const cl_fvector_t& rhs, const cl_fvector_t& lhs);

} /* namespace clf */