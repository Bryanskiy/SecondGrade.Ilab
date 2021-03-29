#include <random>

#include "../kgraph.hpp"

template<typename VT, typename ET>
class graph_generator_t {
public:
    graph_generator_t() : rd_(), gen_(rd_()) {}

    kgraph::kgraph_t<VT, ET> generate_bipartite_graph();
    kgraph::kgraph_t<VT, ET> generate_unbipartite_graph();
    kgraph::kgraph_t<VT, ET> generate_odd_loop();
private:
    std::random_device rd_;
    std::mt19937 gen_;
};

template<typename VT, typename ET>
kgraph::kgraph_t<VT, ET> graph_generator_t<VT, ET>::generate_bipartite_graph() {
    std::uniform_int_distribution<> dis_low(0, std::numeric_limits<int>::max() / 2);
    std::uniform_int_distribution<> dis_height(std::numeric_limits<int>::max() / 2 + 1, std::numeric_limits<int>::max());
    std::uniform_int_distribution<> dis_count(1u, 10000u);

    std::size_t count = dis_count(gen_);

    kgraph::kgraph_t<VT, ET> ret;
    std::size_t height = dis_height(gen_);
    ret.push_edge(1, height);

    for(std::size_t i = 0; i < count; ++i) {
        std::size_t low = dis_low(gen_);
        std::size_t height = dis_height(gen_);

        ret.push_edge(low, height);
    }

    return ret;
}

template<typename VT, typename ET>
kgraph::kgraph_t<VT, ET> graph_generator_t<VT, ET>::generate_odd_loop() {
    kgraph::kgraph_t<VT, ET> ret;
    std::uniform_int_distribution<> dis(0, std::numeric_limits<int>::max() / 2);
    std::uniform_int_distribution<> dis_len(1u, 10000u);

    std::size_t len = dis_len(gen_);
    if((len % 2) == 0) {len += 1;}

    std::size_t v = dis(gen_);
    ret.push_edge(1, v);

    for(std::size_t i = 2; i < len; ++i) {
        std::size_t u = dis(gen_);
        ret.push_edge(v, u);
        v = u;
    }

    ret.push_edge(1, v);

    return ret;
}

int main() {
    graph_generator_t<std::size_t, std::size_t> gen;

    try {
        auto&& graph1 = gen.generate_bipartite_graph();

        auto&& cycle = graph1.fill_bipartite_color(1, kgraph::color_t::blue);
        if(cycle.has_value()) {
            std::cout << "generate_bipartite_graph TEST: FAILED" << std::endl;
        } else {
            std::cout << "generate_bipartite_graph TEST: SUCCESS" << std::endl;
        }

        auto&& graph2 = gen.generate_odd_loop();
        cycle = graph2.fill_bipartite_color(1, kgraph::color_t::blue);

        if(!cycle.has_value()) {
            std::cout << "generate_odd_loop TEST: FAILED" << std::endl;
        } else {
            std::cout << "generate_odd_loop TEST: SUCCESS" << std::endl;
        }

    } catch(std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
}