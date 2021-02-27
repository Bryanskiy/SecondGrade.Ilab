#pragma once

#include <iostream>
#include <vector>
#include <optional>
#include "../matrix/matrix.hpp"

namespace circuit {

/*---------------------------------------------------------------------------------
                                EDGE
-----------------------------------------------------------------------------------*/

class edge_t {
public:

    edge_t(std::size_t v1, std::size_t v2, double resistance, double eds) :
        v1_(v1), v2_(v2), resistance_(resistance), eds_(eds) {}

    void set_id(std::size_t id) {id_ = id;} 
    void set_current(double current) {current_ = current;}

    std::optional<std::size_t> have_incident_vertex(const edge_t& rhs) {
        if      (get_v1() == rhs.get_v1()) { return get_v1(); }
        else if (get_v1() == rhs.get_v2()) { return get_v2(); }
        else if (get_v2() == rhs.get_v1()) { return get_v2(); }
        else if (get_v2() == rhs.get_v2()) { return get_v2(); }  

        return std::optional<std::size_t>();
    }

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

bool operator==(const edge_t& lhs, const edge_t& rhs);

/*---------------------------------------------------------------------------------
                                CIRCUIT 
-----------------------------------------------------------------------------------*/

class circuit_t {
public:
    circuit_t(const std::vector<edge_t>& edges);
    void calculate_currents();
    void print_currents() const;

private:

    std::vector<std::vector<edge_t>> find_cycles() const;       
    std::vector<edge_t> dfs_cycle_handler(const edge_t& edge) const;     
    void dfs_cycle(std::size_t start_v, std::size_t current_v, std::vector<edge_t>& current_pass) const;

private:
    std::vector<edge_t> edges_;
    matrix::matrix_t<int> incidence_matrix_;
};

} /* namespace circuit */ 