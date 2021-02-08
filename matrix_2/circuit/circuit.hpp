#pragma once

#include <iostream>
#include <vector>
#include <variant>
#include "../matrix.h"

namespace circuit {

class edge_t {
public:

    edge_t(std::size_t v1, std::size_t v2, double resistance, double eds) :
        v1_(v1), v2_(v2), resistance_(resistance), eds_(eds) {}

    std::size_t get_v1() {return v1_;}
    std::size_t get_v2() {return v2_;}
    double get_current() {return current_;}
    double get_resistance() {return resistance_;}
    std::variant<double> get_eds() {return eds_;}


private:
    std::size_t v1_, v2_;
    double current_;
    double resistance_;
    std::variant<double> eds_;
};

class circuit_t {
public:

private:
    matrix::matrix_t<bool> incidence_matrix_;
    std::vector<edge_t> edges_;
};

} /* namespace circuit */ 