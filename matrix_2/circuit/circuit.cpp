#include "circuit.hpp"

namespace circuit {

circuit_t::circuit_t(const std::vector<edge_t>& edges) {
    edges_ = edges;

    for(const auto& edge : edges_) {
        std::size_t new_cols = incidence_matrix_.get_cols_number() + 1;
        std::size_t new_rows = std::max(incidence_matrix_.get_rows_number() , std::max(edge.get_v1(), edge.get_v2()));
        incidence_matrix_.resize(new_rows, new_cols);

       incidence_matrix_[edge.get_v1() - 1][incidence_matrix_.get_cols_number() - 1] = 1;
       incidence_matrix_[edge.get_v1() - 1][incidence_matrix_.get_cols_number() - 1] = 1;  
    }
}

} /* namespace circuit */