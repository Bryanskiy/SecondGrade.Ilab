#include "include/geometry.h"

int main() {
    ivkg::vector_t<3> v1{0.0, 4.0, 0.0};
    ivkg::vector_t<3> v2{0.0, 4.0, 0.0};
    v1 == v2;
}