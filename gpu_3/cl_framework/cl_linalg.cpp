#include "cl_linalg.hpp"

namespace clf {

/*-------------------------------------------------------------------------
                        cl_fvector_t REALIZATION
--------------------------------------------------------------------------*/

cl_fvector_t& cl_fvector_t::operator+=(const cl_fvector_t& rhs) {
    std::size_t rhs_size = rhs.size();
    if(size() < rhs_size) {
        data_.resize(rhs_size);
    }

    cl::Buffer lhs_buffer(context_, CL_MEM_WRITE_ONLY, data_.size() * sizeof(data_[0]));
    queue_.enqueueWriteBuffer(lhs_buffer, CL_TRUE, 0, data_.size() * sizeof(data_[0]), data_.data());

    cl::Buffer rhs_buffer(context_, CL_MEM_READ_ONLY, rhs.data_.size() * sizeof(rhs.data_[0]));
    queue_.enqueueWriteBuffer(rhs_buffer, CL_TRUE, 0, rhs.data_.size() * sizeof(rhs.data_[0]) , rhs.data_.data());

    cl::NDRange offset(0);
    cl::NDRange global_size(rhs_size);

    cl::Kernel kernel (program_ , "fvector_add");
    kernel.setArg (0 , lhs_buffer);
    kernel.setArg (1 , rhs_buffer);

    cl::Event event;
    queue_.enqueueNDRangeKernel(kernel, 0, global_size, cl::NullRange, nullptr, &event);
    event.wait();
    queue_.enqueueReadBuffer(lhs_buffer, CL_TRUE, 0, data_.size() * sizeof(data_[0]), data_.data());

    return *this;
}

cl_fvector_t& cl_fvector_t::scalar_mult(float value) {

    cl::Buffer lhs_buffer(context_, CL_MEM_WRITE_ONLY, data_.size() * sizeof(data_[0]));
    queue_.enqueueWriteBuffer(lhs_buffer, CL_TRUE, 0, data_.size() * sizeof(data_[0]), data_.data());

    cl::NDRange offset(0);
    cl::NDRange global_size(data_.size());

    cl::Kernel kernel (program_ , "fvector_scalar_mult");
    kernel.setArg (0 , lhs_buffer);
    kernel.setArg (1 , value);

    cl::Event event;
    queue_.enqueueNDRangeKernel(kernel, 0, global_size, cl::NullRange, nullptr, &event);
    event.wait();
    queue_.enqueueReadBuffer(lhs_buffer, CL_TRUE, 0, data_.size() * sizeof(data_[0]), data_.data());

    return *this;
}

cl_fvector_t& cl_fvector_t::scalar_mult(cl_fvector_t& rhs) {
    std::size_t rhs_size = rhs.size();
    if(size() < rhs_size) {
        data_.resize(rhs_size);
    }

    cl::Buffer lhs_buffer(context_, CL_MEM_WRITE_ONLY, data_.size() * sizeof(data_[0]));
    queue_.enqueueWriteBuffer(lhs_buffer, CL_TRUE, 0, data_.size() * sizeof(data_[0]), data_.data());

    cl::Buffer rhs_buffer(context_, CL_MEM_READ_ONLY, rhs.data_.size() * sizeof(rhs.data_[0]));
    queue_.enqueueWriteBuffer(rhs_buffer, CL_TRUE, 0, rhs.data_.size() * sizeof(rhs.data_[0]) , rhs.data_.data());

    cl::NDRange offset(0);
    cl::NDRange global_size(rhs_size);

    cl::Kernel kernel (program_ , "fvector_mult");
    kernel.setArg (0 , lhs_buffer);
    kernel.setArg (1 , rhs_buffer);

    cl::Event event;
    queue_.enqueueNDRangeKernel(kernel, 0, global_size, cl::NullRange, nullptr, &event);
    event.wait();
    queue_.enqueueReadBuffer(lhs_buffer, CL_TRUE, 0, data_.size() * sizeof(data_[0]), data_.data());

    return *this;
}

bool cl_fvector_t::operator<(const cl_fvector_t& rhs) {
    return data_ < rhs.data_;
}

bool cl_fvector_t::operator>(const cl_fvector_t& rhs) {
    return data_ > rhs.data_;
}

bool cl_fvector_t::operator==(const cl_fvector_t& rhs) {
    return data_ == rhs.data_;
}

bool cl_fvector_t::operator!=(const cl_fvector_t& rhs) {
    return data_ != rhs.data_;
}

const cl_fvector_t operator+(const cl_fvector_t& rhs, const cl_fvector_t& lhs) {
    cl_fvector_t tmp = rhs;
    tmp += lhs;
    return tmp;
}

cl_fvector_t& cl_fvector_t::negate() & {

    cl::Buffer buffer(context_, CL_MEM_WRITE_ONLY, data_.size() * sizeof(data_[0]));
    queue_.enqueueWriteBuffer(buffer, CL_TRUE, 0, data_.size() * sizeof(data_[0]), data_.data());

    cl::NDRange offset(0);
    cl::NDRange global_size(data_.size());

    cl::Kernel kernel (program_ , "fvector_negate");
    kernel.setArg (0 , buffer);

    cl::Event event;
    queue_.enqueueNDRangeKernel(kernel, 0, global_size, cl::NullRange, nullptr, &event);
    event.wait();
    queue_.enqueueReadBuffer(buffer, CL_TRUE, 0, data_.size() * sizeof(data_[0]), data_.data());

    return *this;
}

cl_fvector_t& cl_fvector_t::operator-=(const cl_fvector_t& rhs) {
    std::size_t rhs_size = rhs.size();
    if(size() < rhs_size) {
        data_.resize(rhs_size);
    }

    cl::Buffer lhs_buffer(context_, CL_MEM_WRITE_ONLY, data_.size() * sizeof(data_[0]));
    queue_.enqueueWriteBuffer(lhs_buffer, CL_TRUE, 0, data_.size() * sizeof(data_[0]), data_.data());

    cl::Buffer rhs_buffer(context_, CL_MEM_READ_ONLY, rhs.data_.size() * sizeof(rhs.data_[0]));
    queue_.enqueueWriteBuffer(rhs_buffer, CL_TRUE, 0, rhs.data_.size() * sizeof(rhs.data_[0]) , rhs.data_.data());

    cl::NDRange offset(0);
    cl::NDRange global_size(rhs_size);

    cl::Kernel kernel (program_ , "fvector_sub");
    kernel.setArg (0 , lhs_buffer);
    kernel.setArg (1 , rhs_buffer);

    cl::Event event;
    queue_.enqueueNDRangeKernel(kernel, 0, global_size, cl::NullRange, nullptr, &event);
    event.wait();
    queue_.enqueueReadBuffer(lhs_buffer, CL_TRUE, 0, data_.size() * sizeof(data_[0]), data_.data());

    return *this;
}

const cl_fvector_t operator-(const cl_fvector_t& rhs, const cl_fvector_t& lhs) {
    cl_fvector_t tmp = rhs;
    tmp -= lhs;
    return tmp;
}

/*-------------------------------------------------------------------------
                        cl_imatrix_t REALIZATION
--------------------------------------------------------------------------*/

cl_imatrix_t::~cl_imatrix_t() {}

/*-------------------------------------------------------------------------
                 cl_bandet_sparce_matrix_t REALIZATION
--------------------------------------------------------------------------*/

cl_bandet_sparce_fmatrix_t::cl_bandet_sparce_fmatrix_t(matrix::matrix_t<float>& matrix) {
    std::size_t rows = matrix.get_rows_number();
    std::size_t cols = matrix.get_cols_number();
    std::size_t max_size = std::max(rows, cols);

    matrix::matrix_t<float> tmp = matrix;
    tmp.resize(max_size, max_size);

    /* process lower triangle */
    for(std::size_t i = 0; i < max_size; ++i) {
        for(std::size_t j = 0; j < max_size - i; ++j) {
            if(tmp[i + j][j] != 0.0) {
                indices_.push_back(i);

                std::vector<float> vector(max_size);

                for(std::size_t k = 0; k < max_size - i; ++k) {
                    vector[k] = tmp[i + k][k];
                }

                diagonals_.emplace_back(vector);
                break;
            }
        }
    }

    /* process upper triangle */
    for(std::size_t i = 1; i < max_size; ++i) {
        for(std::size_t j = 0; j < max_size - i; ++j) {
            if(tmp[j][j + i] != 0.0) {
                indices_.push_back(i + max_size);

                std::vector<float> vector(max_size);

                for(std::size_t k = 0; k < max_size - i; ++k) {
                    vector[k + i] = tmp[k][k + i];
                }

                diagonals_.emplace_back(vector);
                break;
            }
        }
    }
}

cl_fvector_t cl_bandet_sparce_fmatrix_t::vector_mult(cl_fvector_t& rhs) {
    if(cols() != rhs.size()) {
        throw std::runtime_error("cl_bandet_sparce_fmatrix_t::vector_mult: invalid sizes");
    }

    std::size_t vector_size = rhs.size();
    cl_fvector_t ret(vector_size);

    for(auto&& diagonal : diagonals_) {
        ret += diagonal.scalar_mult(rhs);
    }

    return ret;
}

} /* namespace clf */