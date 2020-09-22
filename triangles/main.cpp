#include <iostream>
#include <vector>
#include "geometry.h"

int main() {
    int n; std::cin >> n;
    std::vector<triangle_t> triangles(n);
    for(auto& i : triangles) {
        std::cin >> i;
    }

    for(int i = 0; i < n - 1; ++i) {
        for(int j = i + 1; j < n; ++j) {
            if(triangles[i].intersect(triangles[j])) {
                std::cout << i << ' ' << j << ' ';
            }
        }
    }

}