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

    std::size_t get_v1() const {return v1_;}
    std::size_t get_v2() const {return v2_;}
    double get_current() const {return current_;}
    double get_resistance() const {return resistance_;}
    double get_eds() const {return eds_;}

private:
    std::size_t v1_, v2_;
    double resistance_;
    double eds_;
    double current_;
};

class circuit_t {
public:
    circuit_t(const std::vector<edge_t>& edges);

private:
    std::vector<edge_t> edges_;
    matrix::matrix_t<int> incidence_matrix_;

#ifdef DEBUG
public:
    void dump() { std::cout << incidence_matrix_ << std::endl;}
#endif
};

} /* namespace circuit */ 