#include "gpu_linalg.hpp"

namespace gpula {

/* ---------------------------FLOAT--------------------------- */

cl_float_t& cl_float_t::operator+=(cl_float_t& rhs) {
    cl::Kernel kernel(program_, "vector_sub");
    cl::Image1D image;
    image.getImageInfo<>
}

} /* namespace gpula */    