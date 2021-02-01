#pragma once

#include "general.h"

namespace lingeo {
    long double signed_distance(const point_t<3>& point, const plane_t& plane);
    long double signed_distance(const plane_t& plane, const point_t<3>& point);

    long double distance(const line_t<3>& lhs, const line_t<3>& rhs);
}