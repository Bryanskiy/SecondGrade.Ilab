#pragma once 

#include <string>

namespace clf {

const std::string wired_kernels = "\
__kernel void fvector_add(__global float* lhs, __global float* rhs){\
    uint id = get_global_id(0);\
    lhs[id] += rhs[id];\
}\
\
__kernel void fvector_constant_mult(__global float* vec, float scalar) {\
    uint id = get_global_id(0);\
    vec[id] *= scalar;\
}\
\
__kernel void fvector_byelement_mult(__global float* lhs, __global float* rhs) {\
    uint id = get_global_id(0);\
    lhs[id] *= rhs[id];\
}\
\
__kernel void fvector_negate(__global float* vec) {\
    uint id = get_global_id(0);\
    vec[id] = -vec[id];\
}\
\
__kernel void fvector_sub(__global float* lhs, __global float* rhs) {\
    uint id = get_global_id(0);\
    lhs[id] -= rhs[id];\
}";

} /* namespace clf */