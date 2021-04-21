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

const cl_fvector_t::cl_fvector_t operator+(const cl_fvector_t& rhs, const cl_fvector_t& lhs) {
    cl_fvector_t tmp = rhs;
    return tmp += lhs;
}

} /* namespace clf */