#include <iostream>
#include <iomanip>

#include "parser/driver.hpp"
#include "kgraph.hpp"


int main(int argc, char** argv) {
    yy::driver_t driver(argv[1]);
    driver.parse();
    std::vector<yy::edge_t> edges = driver.get_edges();

    kgraph::kgraph_t<std::size_t, std::size_t> graph;

    for(auto&& edge : edges) {
        graph.push_vertex(edge.v1);
        graph.push_vertex(edge.v2);
        graph.push_edge(edge.v1, edge.v2, edge.w);
    }

    graph.dump(std::cout);
}