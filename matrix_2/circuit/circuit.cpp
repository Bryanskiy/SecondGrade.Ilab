#include "circuit.hpp"

namespace circuit {

/*---------------------------------------------------------------------------------
                                CIRCUIT 
-----------------------------------------------------------------------------------*/


circuit_t::circuit_t(const matrix::matrix_t<double>& resistance_matrix, const matrix::matrix_t<double>& eds_matrix, const matrix::matrix_t<int>& edges_matrix) {
    
    incidence_matrix_.resize(edges_matrix.get_rows_number(), edges_matrix.get_cols_number());
    for(std::size_t i = 0, maxi = incidence_matrix_.get_rows_number(); i < maxi; ++i) {
        for(std::size_t j = 0, maxj = incidence_matrix_.get_cols_number(); j < maxj; ++j) {
            if(edges_matrix[i][j]) {
                incidence_matrix_[i][j] = 1u;
            }
        }
    }

    for(std::size_t i = 0, maxi = incidence_matrix_.get_cols_number(); i < maxi; ++i) {
        std::size_t v1 = 0u, v2 = 0u;

        bool flag = false;
        for(std::size_t j = 0, maxj = incidence_matrix_.get_rows_number(); j < maxj; ++j) {
            if(edges_matrix[j][i] == 1) {
                v1 = j;    
            }
            
            else if (edges_matrix[j][i] == 2) {
                v2 = j;
            }

        }

        if(v1 == 0) {
            v1 = v2;
        } else if(v2 == 0) {
            v2 = v1;
        }

        double resistance = resistance_matrix[v1][i];
        double eds = eds_matrix[v1][i];

        edges_.push_back({v1, v2, resistance, eds});
        edges_.back().set_id(i);
    }
}

matrix::matrix_t<double> circuit_t::get_currents() const {
    matrix::matrix_t<double> ret(incidence_matrix_.get_rows_number(), incidence_matrix_.get_cols_number());
    for(std::size_t i = 0, maxi = ret.get_rows_number(); i < maxi; ++i) {
        for(std::size_t j = 0, maxj = ret.get_cols_number(); j < maxj; ++j) {
            ret[i][j] = (incidence_matrix_[i][j]) ? edges_[j].get_current() : 0.0;
        }
    }

    return ret;
}

bool circuit_t::calculate_currents() {
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
    if(ret.first.get_elements_number() == 0) {
        return false;
    }

    for(std::size_t i = 0, maxi = ret.first.get_rows_number(); i < maxi; ++i) {
        edges_[i].set_current(ret.first[i][0]);
    }

    return true;
}

std::vector<std::vector<circuit_t::edge_t>> circuit_t::find_cycles() const {
    std::vector<std::vector<edge_t>> ret;
    for(const auto& edge : edges_) {
        auto cycle = dfs_cycle_handler(edge);
        if(!cycle.empty()) {
            ret.push_back(cycle);
        }
    }

    return ret;
}

std::vector<circuit_t::edge_t> circuit_t::dfs_cycle_handler(const edge_t& edge) const {
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
        if(incidence_matrix_[current_v][i]) {

            /* check if we come from this edge */
            if(prev_edge.get_id() == edges_[i].get_id()) {
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