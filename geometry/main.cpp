
#include "src/include/geometry.h"
#include "src/include/octtree.h"

#include <vector>

int main() {
    std::size_t N; std::cin >> N;
    std::vector<lingeo::triangle_t<3>> triangles(N);
    for(std::size_t i = 0; i < N; ++i) {
        std::cin >> triangles[i];
    }

    octt::space_t space = octt::create(triangles);
    octt::octtree_t<lingeo::triangle_t<3>> octtree(space);

    for(std::size_t i = 0; i < N; ++i) {
        octtree.insert(i, triangles[i]);
    }

    octtree.print_intersections(std::cout);
}