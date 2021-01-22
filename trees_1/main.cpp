#include <iostream>
#include <set>
#include <vector>
#include <utility>
#include "avl_tree.hpp"

void execute_set(const avl::tree_t<int>& set, const std::vector<std::pair<int, int>>& requests) {
    for(const auto& request : requests) {
        auto left_it = set.lower_bound(request.first);
        auto right_it = set.upper_bound(request.second);

        std::size_t count = 0;
        while (left_it != right_it) {
            ++left_it;
            ++count;
        }
        std::cout << count << ' ';
    }    
}

int main() {
    int N; std::cin >> N;
    avl::tree_t<int> avl_set;
    for(std::size_t i = 0; i < N; ++i) {
        int elem; std::cin >> elem;
        avl_set.insert(elem);
    }

    int M; std::cin >> M;
    std::vector<std::pair<int, int>> requests(M);
    for(auto& request : requests) {
        std::cin >> request.first;
        std::cin >> request.second;
    }

    execute_set(avl_set, requests);
}