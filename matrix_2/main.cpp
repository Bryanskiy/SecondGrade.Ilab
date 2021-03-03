#include <iostream>
#include <vector>
#include <FlexLexer.h>

#include "circuit/circuit.hpp"
#include "matrix/matrix.hpp"
#include "parser/driver.hpp"

int main() {
    yy::driver_t driver;
    driver.parse();

    auto resistance_data = driver.get_resistance_matrix();
    auto eds_data = driver.get_eds_matrix();
    auto edges_data = driver.get_edges_matrix();

    circuit::circuit_t circuit(resistance_data, eds_data, edges_data);
    bool flag = circuit.calculate_currents();
    if(!flag) {
        std::cout << "Can't calculate currents" << std::endl;
        return 0;
    }   
    auto current_data = circuit.get_currents();

    /* print ans */
    for(std::size_t i = 0, maxi = current_data.get_cols_number(); i < maxi; ++i) {
        std::size_t v1 = 0u, v2 = 0u;

        bool flag = false;
        for(std::size_t j = 0, maxj = current_data.get_rows_number(); j < maxj; ++j) {
            if(edges_data[j][i] == 1) {
                v1 = j;    
            }
            
            else if (edges_data[j][i] == 2) {
                v2 = j;
            }
        }

        if(v1 == 0) {
            v1 = v2;
        } else if(v2 == 0) {
            v2 = v1;
        }

        double current = current_data[v1][i];
        std::cout << v1 << " -- " << v2 << ": " << current << " A" << std::endl;
    }
}