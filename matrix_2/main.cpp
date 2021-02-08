#include <iostream>
#include <vector>
#include <FlexLexer.h>

#include "circuit/circuit.hpp"
#include "matrix.h"
#include "parser/driver.hpp"

int main(int argc, char** argv) {
    yy::driver_t driver(argv[1]);
    driver.parse();
    std::vector<circuit::edge_t> edges = driver.get_edges();
    
}