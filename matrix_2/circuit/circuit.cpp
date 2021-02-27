#include "circuit.hpp"

namespace circuit {

/*---------------------------------------------------------------------------------
                                EDGE
-----------------------------------------------------------------------------------*/

bool operator==(const edge_t& lhs, const edge_t& rhs) {
    return lhs.get_id() == rhs.get_id();
}

/*---------------------------------------------------------------------------------
                                CIRCUIT 
-----------------------------------------------------------------------------------*/


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
    std::vector<std::vector<edge_t>> independent_cycles = find_cycles();   
    std::size_t independent_cycles_count = independent_cycles.size();
#ifdef DEBUG
    std::cout << "----------------------Independent cycles----------------------" << std::endl;
    for(std::size_t i = 0; i < independent_cycles_count; ++i) {
        std::cout << "[" << i << "]: ";
        for(std::size_t j = 0, maxj = independent_cycles[i].size(); j < maxj; ++j) {
            std::cout << independent_cycles[i][j].get_v1() << " -- " << independent_cycles[i][j].get_v2() << " ";
        }
        std::cout << std::endl;
    }
#endif 

    matrix::matrix_t<double> system(incidence_matrix_.get_rows_number() + independent_cycles_count - 1, edges_.size());
    matrix::matrix_t<double> right(system.get_rows_number(), 1);
    
    /*second rule*/
    for(std::size_t i = 0, maxi = independent_cycles_count; i < maxi; ++i) {
        edge_t prev_edge = independent_cycles[i].back();
        double eds = 0.0;
        float sign = 1.0;
        for(std::size_t j = 0, maxj = independent_cycles[i].size(); j < maxj; ++j) {
            edge_t curr_edge = independent_cycles[i][j];

            std::size_t incidence_vertex = curr_edge.have_incident_vertex(prev_edge).value();

            if((prev_edge.get_v1() == incidence_vertex) && (curr_edge.get_v1() == incidence_vertex)) {
                sign *= -1.0;
            } else if((prev_edge.get_v2() == incidence_vertex) && (curr_edge.get_v2() == incidence_vertex)) {
                sign *= -1.0;
            }

            system[i][curr_edge.get_id()] = curr_edge.get_resistance() * sign;
            eds += curr_edge.get_eds() * sign;
            prev_edge = curr_edge;
        }
        right[i][0] = eds;
    }


    /*first rule*/
    for (std::size_t i = 1u, maxi = incidence_matrix_.get_rows_number(); i < maxi; ++i) {
        for(std::size_t j = 0u, maxj = incidence_matrix_.get_cols_number(); j < maxj; ++j) {
            if (incidence_matrix_[i][j]) {
                if(edges_[j].get_v1() != edges_[j].get_v2()) {
                    system[independent_cycles_count + i - 1][j] = ((edges_[j].get_v2() == i) ? 1.0 : -1.0);
                } 
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

std::vector<std::vector<edge_t>> circuit_t::find_cycles() const {
    std::vector<std::vector<edge_t>> ret;
    for(const auto& edge : edges_) {
        auto cycle = dfs_cycle_handler(edge);
        if(!cycle.empty()) {
            ret.push_back(cycle);
        }
    }

    return ret;
}

std::vector<edge_t> circuit_t::dfs_cycle_handler(const edge_t& edge) const {
    std::vector<edge_t> ret{edge};
    dfs_cycle(edge.get_v1(), edge.get_v2(), ret);
    return ret;
}    


void circuit_t::dfs_cycle(std::size_t start_v, std::size_t current_v, std::vector<edge_t>& current_pass) const
{
    if((start_v == current_v) && (!current_pass.empty())) {
        return;
    }


    edge_t prev_edge = current_pass.back();

    for(std::size_t i = 0, max = incidence_matrix_.get_cols_number(); i < max; ++i) {
        /* if pass to i edge exist */
        if(incidence_matrix_[current_v][i] == 1) {

            /* check if we come from this edge */
            if(prev_edge == edges_[i]) {
                continue;
            }

            /* check if we were here */
            bool flag = false;
            for(const auto& edge : current_pass) {
                if(i == edge.get_id()) {
                    flag = true;
                    break;
                }
            }
            if(flag) {
                continue;
            }

            std::vector<edge_t> copy_pass = current_pass;
            copy_pass.push_back(edges_[i]);

            std::size_t next_v = (edges_[i].get_v1() == current_v) ? edges_[i].get_v2() : edges_[i].get_v1();
            dfs_cycle(start_v, next_v, copy_pass);

            if(!copy_pass.empty()) {
                current_pass = copy_pass;
                return;
            }
        }

    }

    /* didn't find independent cycle */
    current_pass = std::vector<edge_t>();
    return;
}

} /* namespace circuit */