#pragma once 

#include <string>

namespace clfm {

const std::string kernels = "\
__kernel void fVectorAdd(__global float* lhs, __global float* rhs){\
    uint id = get_global_id(0);\
    lhs[id] += rhs[id];\
}\
\
__kernel void fVectorConstantMult(__global float* vec, float scalar) {\
    uint id = get_global_id(0);\
    vec[id] *= scalar;\
}\
\
__kernel void fVectorMult(__global float* lhs, __global float* rhs) {\
    uint id = get_global_id(0);\
    lhs[id] *= rhs[id];\
}\
\
__kernel void fVectorSub(__global float* lhs, __global float* rhs) {\
    uint id = get_global_id(0);\
    lhs[id] -= rhs[id];\
}";

} /* namespace clfm */