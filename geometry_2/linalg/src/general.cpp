#include "../include/primitives/general.h"

namespace lingeo {

    bool valid(const coordinate_t& coordinate) {
        return !(std::isnan(coordinate) || !std::isfinite(coordinate));
    }

    bool equal(const coordinate_t& lhs, const coordinate_t& rhs) {
        return valid(lhs) && valid(rhs) && (std::abs(lhs - rhs) < tolerance);
    }

    int sign(long double x) {
        if(x > tolerance) {return 1;}
        if(x < -tolerance) {return -1;}
        else {return 0;}
    }

    void swap(long double& lhs, long double& rhs) {
        long double tmp = lhs;
        lhs = rhs;
        rhs = tmp;
    }
}    