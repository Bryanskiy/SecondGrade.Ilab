#include "circuit.hpp"

namespace circuit {

circuit_t::circuit_t(const std::vector<edge_t>& edges) : edges_(edges) {
    std::size_t i = 0;
    for(const auto& edge : edges_) {
        edges_[i].set_id(i);
        std::size_t new_cols = incidence_matrix_.get_cols_number() + 1;
        std::size_t new_rows = std::max(incidence_matrix_.get_rows_number() , std::max(edge.get_v1(), edge.get_v2() + 1));
        incidence_matrix_.resize(new_rows, new_cols);

        incidence_matrix_[edge.get_v1()][incidence_matrix_.get_cols_number() - 1] = 1;
        incidence_matrix_[edge.get_v2()][incidence_matrix_.get_cols_number() - 1] = 1;
        ++i;
    }
}

void circuit_t::print_currents() const {
    for(const auto& edge : edges_) {
        std::cout << edge.get_v1() << " -- " << edge.get_v2() << ": " << edge.get_current() << " A" << std::endl; 
    }
}

void circuit_t::calculate_currents() {
    std::vector<std::vector<edge_t>> independent_cycles = find_independent_cycles();
    std::size_t independent_cycles_count = independent_cycles.size();
    matrix::matrix_t<double> system(incidence_matrix_.get_rows_number() + independent_cycles_count - 2, edges_.size());
    matrix::matrix_t<double> right(system.get_rows_number(), 1);

    /*second rule*/
    for(std::size_t i = 0, maxi = independent_cycles_count; i < maxi; ++i) {
        edge_t prev_edge = independent_cycles[0].back();
        double eds = 0.0;
        bool minus = false;
        for(std::size_t j = 0, maxj = independent_cycles[i].size(); j < maxj; ++j) {
            edge_t curr_edge = independent_cycles[i][j];

            if((j != 0) && (prev_edge.get_v2() != curr_edge.get_v1())) {
                minus = !minus;
            }

            system[i][curr_edge.get_id()] = curr_edge.get_resistance() * (minus ? (-1.0) : (1.0));
            eds += curr_edge.get_eds() * (minus ? (-1.0) : (1.0));
            prev_edge = curr_edge;
        }
        right[i][0] = eds;
    }


    /*first rule*/
    for (std::size_t i = 2u, maxi = incidence_matrix_.get_rows_number(); i < maxi; ++i) {
        for(std::size_t j = 0u, maxj = incidence_matrix_.get_cols_number(); j < maxj; ++j) {
            if (incidence_matrix_[i][j]) {
                system[independent_cycles_count + i - 2][j] = ((edges_[j].get_v2() == i) ? 1.0 : -1.0);
            }
        }
    }
#ifdef DEBUG
    std::cout << "----------------------System----------------------" << std::endl;
    std::cout << "Left: " << std::endl;
    std::cout << system << std::endl;
    std::cout << "Right: " << std::endl;
    std::cout << right << std::endl;
#endif
    auto ret = matrix::solve_linear_system(system, right);
    for(std::size_t i = 0, maxi = ret.first.get_rows_number(); i < maxi; ++i) {
        edges_[i].set_current(ret.first[i][0]);
    }

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