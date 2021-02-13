#include "circuit.hpp"

namespace circuit {

circuit_t::circuit_t(const std::vector<edge_t>& edges) : edges_(edges) {
    for(const auto& edge : edges_) {
        std::size_t new_cols = incidence_matrix_.get_cols_number() + 1;
        std::size_t new_rows = std::max(incidence_matrix_.get_rows_number() , std::max(edge.get_v1(), edge.get_v2() + 1));
        incidence_matrix_.resize(new_rows, new_cols);

        incidence_matrix_[edge.get_v1()][incidence_matrix_.get_cols_number() - 1] = 1;
        incidence_matrix_[edge.get_v2()][incidence_matrix_.get_cols_number() - 1] = 1;  
    }

    std::cout << incidence_matrix_ << std::endl;
}

void circuit_t::calculate_currents() {
    std::vector<std::vector<edge_t>> independet_loops = find_independent_cycles();
}

std::vector<std::vector<edge_t>> circuit_t::find_independent_cycles() const {
    std::vector<std::vector<edge_t>> ret;
    std::vector<int> marks(edges_.size());
    std::size_t current_vertex = 0u;
    std::size_t scip_count = 0u;
    while(1) {
        if(scip_count == incidence_matrix_.get_rows_number()) {
            return ret;
        }

        std::size_t next_vertex = (current_vertex + 1) % incidence_matrix_.get_rows_number();
        std::vector<edge_t> current_pass;
        dfs_cycle_handler(next_vertex, current_vertex, next_vertex, current_pass, marks);
        if(!current_pass.size()) {
            ++scip_count;
        } else {
            ret.push_back(current_pass);
            scip_count = 0u;
        }

        current_vertex = next_vertex;
    }

    return std::vector<std::vector<edge_t>>();
}

void circuit_t::dfs_cycle_handler(std::size_t start_v, std::size_t prev_v, std::size_t current_v, 
                   std::vector<edge_t>& current_pass, std::vector<int>& marks) const 
{
    dfs_cycle(start_v, prev_v, current_v, false, current_pass, marks);
}

void circuit_t::dfs_cycle(std::size_t& start_v, std::size_t& prev_v, std::size_t& current_v, bool include_independent_edge, 
                                         std::vector<edge_t>& current_pass, std::vector<int>& marks) const 
{
    if((current_v == start_v) && (current_pass.size())) {
        return;
    }

    for(std::size_t i = 0, max = incidence_matrix_.get_cols_number(); i < max; ++i) {
        /* if pass to i edge exist */
        if(incidence_matrix_[current_v][i] == 1) {

            /* check if we come from this edge */
            if((edges_[i].get_v1() == prev_v) || (edges_[i].get_v2() == prev_v)) {
                continue;
            }

            if(!include_independent_edge && marks[i]) {
                continue;
            }

            current_pass.push_back(edges_[i]);
            marks[i] = 1;
            prev_v = current_v;
            current_v = (edges_[i].get_v1() == current_v) ? edges_[i].get_v2() : edges_[i].get_v1();
            dfs_cycle(start_v, prev_v, current_v, true, current_pass, marks);
            if(current_v == start_v) {
                return;
            }
        }

    }

    /* didn't find independent cycle */
    current_pass = std::vector<edge_t>();
    return;
}

} /* namespace circuit */