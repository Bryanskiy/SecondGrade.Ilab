#include "../../circuit/circuit.hpp"
#include <random>

/* 
 * class for generating random circuits
 * now, it generate only 1 kind of circuits
 */
class curcuit_generator {
public:
    curcuit_generator() : rd_(), gen_(rd_()) {}

    std::vector<circuit::edge_t> generate_cycle_tree_circuit(std::size_t n, std::size_t deep, std::size_t leaf_count);
private:

    void connect_tree(std::size_t vertex, std::size_t max_deep, std::size_t current_deep, std::size_t& available_idx, std::vector<circuit::edge_t>& edges, std::size_t leaf_count);

    std::random_device rd_;
    std::mt19937 gen_;
};

/* 
 * DESCRIOPTION:
 *     this func generate circuit with cycle in 1-2-...-n vertices and 1 eds on it,
 *     from every vertex it generate tree with random resistance 
 * RETURN:
 *     vector of edges with correct currents
 *     call circuit calculation and check that currents didn't changes
 */
std::vector<circuit::edge_t> curcuit_generator::generate_cycle_tree_circuit(std::size_t n, std::size_t deep, std::size_t leaf_count) {
    std::vector<circuit::edge_t> ret;

    /* first edge with eds */
    ret.push_back({1u, 2u, 1.0, static_cast<double>(n), 1.0});

    for(std::size_t i = 2; i < n; ++i) {
        ret.push_back({i, i + 1, 1.0, 0.0, 1.0});
    }

    /* connect last elements */
    ret.push_back({n, 1, 1.0, 0.0, 1.0});

    std::size_t avaible_idx = n + 1;
    for(std::size_t i = 1; i <= n; ++i) {
        connect_tree(i, deep, 0, avaible_idx, ret, leaf_count);
    }

    return ret;
}

void curcuit_generator::connect_tree(std::size_t vertex, std::size_t max_deep, 
                                     std::size_t current_deep, std::size_t& available_idx, std::vector<circuit::edge_t>& edges, std::size_t leaf_count) 
{
    if(current_deep == max_deep) {
        return;
    }

    std::uniform_real_distribution<> dis(0.0, 100.0);

    std::size_t avidx_copy_1 = available_idx;
    for(std::size_t i = 0; i < leaf_count; ++i) {
        edges.push_back({vertex, available_idx, dis(gen_), 0.0, 0.0});
        ++available_idx;
    }
    std::size_t avidx_copy_2 = available_idx;

    for(std::size_t i = avidx_copy_1; i < avidx_copy_2; ++i) {
        connect_tree(i, max_deep, current_deep + 1, available_idx, edges, leaf_count);
    }
}

void run_test(std::size_t n, std::size_t deep, std::size_t leaf_count) {
    curcuit_generator gen;
    auto test = gen.generate_cycle_tree_circuit(n, deep, leaf_count);
    auto ans = test;
    circuit::circuit_t circuit(test);
    circuit.calculate_currents();
    auto calculated_ret = circuit.get_edges();
    for(std::size_t i = 0; i < calculated_ret.size(); ++i) {
        if(std::abs(ans[i].get_current() - calculated_ret[i].get_current()) > 10e-5) {
            std::cerr << "Random cycle-trees circuit test[n = "<< n << ", deep = " << deep << ", leaf_count = " << leaf_count << "]: FAILED" << std::endl;
            break;
        }
    }
    std::cerr << "Random cycle-trees circuit test[n = "<< n << ", deep = " << deep << ", leaf_count = " << leaf_count << "]: SUCCESS" << std::endl;
}

int main() {
    run_test(10, 4, 3);
    run_test(3, 8, 2);
}