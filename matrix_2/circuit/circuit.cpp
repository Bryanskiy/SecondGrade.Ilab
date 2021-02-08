#pragma once

#include "circuit.hpp"

namespace circuit {

circuit_t::circuit_t(const std::vector<edge_t>& edges) {
    edges_ = edges;

    for(const auto& edge : edges_) {
        
    }
}

} /* namespace circuit */