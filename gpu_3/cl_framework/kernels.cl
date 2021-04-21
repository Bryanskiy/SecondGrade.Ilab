__global fvector_add(__global float* lhs, __global float* rhs) {
    uint id = get_global_id (0);
    lhs[id] += rhs[id];
}