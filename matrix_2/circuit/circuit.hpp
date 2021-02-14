#pragma once

#include <iostream>
#include <vector>
#include <variant>
#include "../matrix/matrix.hpp"

namespace circuit {

class edge_t {
public:

    edge_t(std::size_t v1, std::size_t v2, double resistance, double eds) :
        v1_(v1), v2_(v2), resistance_(resistance), eds_(eds) {}

    void set_id(std::size_t id) {id_ = id;} 
    void set_current(double current) {current_ = current;}

    std::size_t get_id() const {return id_;}
    std::size_t get_v1() const {return v1_;}
    std::size_t get_v2() const {return v2_;}
    double get_current() const {return current_;}
    double get_resistance() const {return resistance_;}
    double get_eds() const {return eds_;}

private:
    std::size_t id_;
    std::size_t v1_, v2_;
    double resistance_;
    double eds_;
    double current_;
};

class circuit_t {
public:
    circuit_t(const std::vector<edge_t>& edges);
    void calculate_currents();
    void print_currents() const;

private:
    /*
     * Definition : a loop is said to be independent if it contains at least one branch which is not a part of any other independent loop.
     * 
     *  this definition suggests an algorithm :
     * 
     * 1) in infinity loop recursive call dfs to the next vertex (1 -> 2 -> ... -> last -> 1 -> ... )
     * 2) use depth-first search for find independent cycle
     * 3) mark every edge in independed loop
     * 4) at least 1 time need to go to the edge, that is not present in the currently found independent cycles (i call it independent edge)
     * 5) if we can't go anywere from all vertices - end
    */
    std::vector<std::vector<edge_t>> find_independent_cycles() const;
    void dfs_cycle(std::size_t& start_v, std::size_t& prev_v, std::size_t& current_v, bool include_independent_edge, 
                   std::vector<edge_t>& current_pass, std::vector<int>& marks) const;

    void dfs_cycle_handler(std::size_t start_v, std::size_t prev_v, std::size_t current_v,  
                   std::vector<edge_t>& current_pass, std::vector<int>& marks) const;               

private:
    std::vector<edge_t> edges_;
    matrix::matrix_t<int> incidence_matrix_;
};

} /* namespace circuit */ 