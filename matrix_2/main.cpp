#include <iostream>
#include <vector>
#include <FlexLexer.h>

#include "circuit/circuit.hpp"
#include "matrix/matrix.hpp"
#include "parser/driver.hpp"

int main(int argc, char** argv) {
    yy::driver_t driver(argv[1]);
    driver.parse();
    std::vector<circuit::edge_t> edges = driver.get_edges();
    circuit::circuit_t circuit(edges);
    circuit.calculate_currents();   
    circuit.print_currents();                    
}