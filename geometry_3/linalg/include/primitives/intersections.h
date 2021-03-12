#pragma once

#include "general.h"

namespace lingeo {
    line_t<3> intersection(const plane_t& lhs, const plane_t& rhs);
    bool intersection(const triangle_t<3>& lhs, const triangle_t<3>& rhs);
    bool intersection(const segment_t<3>& lhs, const segment_t<3>& rhs);
    bool intersection(const segment_t<3>& s, const point_t<3>& p);
}
