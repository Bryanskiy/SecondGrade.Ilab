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
        dfs_cycle_handler(next_vertex, next_vertex, current_pass, marks);
        if(!current_pass.size()) {
            ++scip_count;
        } else {
            ret.push_back(current_pass);
            scip_count = 0u;
            for(std::size_t i = 0, maxi = current_pass.size(); i < maxi; ++i) {
                marks[current_pass[i].get_id()] = 1;
            }
        }

        current_vertex = next_vertex;
    }

    return std::vector<std::vector<edge_t>>();
}

void circuit_t::dfs_cycle_handler(std::size_t start_v, std::size_t current_v, 
                   std::vector<edge_t>& current_pass, const std::vector<int>& marks) const 
{
    dfs_cycle(start_v, current_v, false, current_pass, marks);
    if(!current_pass.size()) {
        return;
    }
    /* cut : dfs find cycle with start_v vertex, but this cycle can contain more cycles, so we cut it */
    for(std::size_t i = 2, maxi = current_pass.size() - 1; i < maxi ;++i) {
        for(std::size_t j = 0, maxj = i - 1; j < maxj ;++j) {
            if(current_pass[j].have_incident_vertex(current_pass[i])) {
                if(j == 0) {
                    current_pass = std::vector<edge_t>(current_pass.begin() + j + 1, current_pass.begin() + i + 1);
                } else {
                    current_pass.erase(current_pass.begin() + j + 1, current_pass.begin() + i + 1);
                    i = 2;
                    j = 0;
                }    
                return;
            }
        }
    }
}

void circuit_t::dfs_cycle(std::size_t start_v, std::size_t& current_v, bool independent_edge, 
                          std::vector<edge_t>& current_pass, const std::vector<int>& marks) const 
{
    if((current_v == start_v) && (current_pass.size())) {
        return;
    }

    std::size_t prev_edge = edges_.size();
    if(current_pass.size()) {
        prev_edge = current_pass.back().get_id();
    }

    for(std::size_t i = 0, max = incidence_matrix_.get_cols_number(); i < max; ++i) {
        /* if pass to i edge exist */
        if(incidence_matrix_[current_v][i] == 1) {

            /* check if we come from this edge */
            if(prev_edge == i) {
                continue;
            }

            /* check if we didn't find independent edge */
            if(!independent_edge && marks[i]) {
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
            std::size_t current_v_copy = current_v;
            current_v = (edges_[i].get_v1() == current_v) ? edges_[i].get_v2() : edges_[i].get_v1();
            dfs_cycle(start_v, current_v, true, copy_pass, marks);

            if(!copy_pass.size()) {
                current_v = current_v_copy;
                continue;
            }

            if(current_v == start_v) {
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