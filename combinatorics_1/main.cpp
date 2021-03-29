#include <iostream>
#include <iomanip>
#include <fstream>

#include "kgraph.hpp"

struct edge_t {
    std::size_t v1, v2, w;
};

std::vector<edge_t> read_input(std::istream& in) {
    std::vector<edge_t> edges;
    while(in) {
        edge_t tmp;
        in >> tmp.v1;
        if(!in) { break; } 
        in.ignore(3);
        in >> tmp.v2; 
        in.ignore(1);
        in >> tmp.w;
        edges.push_back(tmp);
    }

    return edges;
}

int main(int argc, char** argv) {
    std::ifstream input(argv[1]);
    std::vector<edge_t> edges = read_input(input);

    kgraph::kgraph_t<std::size_t, std::size_t> graph;

    std::optional<std::vector<std::size_t>> possible;
    try {
        for(auto&& edge : edges) {
            graph.push_edge(edge.v1, edge.v2, edge.w);
        }

        possible = graph.fill_bipartite_color();
    }  catch(std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    if(!possible.has_value()) {
        auto&& ans = graph.get_color();

        for(auto&& elem : ans) {
            std::cout << elem.first << " " << kgraph::color_t::get_string(elem.second) << " ";
        }
    } else {
        for(auto&& elem : possible.value()) {
            std::cout << elem << " ";
        }
    }
}