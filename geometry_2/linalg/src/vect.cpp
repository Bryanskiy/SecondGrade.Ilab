#include "../include/primitives/vect.h"

lingeo::vector_t<3> lingeo::cross(const lingeo::vector_t<3>& lhs, const lingeo::vector_t<3>& rhs) {
    lingeo::coordinate_t x = lhs[1] * rhs[2] - lhs[2] * rhs[1];
    lingeo::coordinate_t y = lhs[2] * rhs[0] - lhs[0] * rhs[2];
    lingeo::coordinate_t z = lhs[0] * rhs[1] - lhs[1] * rhs[0];
    return lingeo::vector_t<3>({x, y, z});
}