#include "../include/octtree.h"

/* -----------------------
SPACE CLASS
-------------------------*/
octt::space_t::space_t() {
    for(std::size_t i = 0; i < 6; i += 2) {
        borders_[i] = std::numeric_limits<long double>::min();
        borders_[i + 1] = std::numeric_limits<long double>::max();
    }
}

octt::space_t::space_t(std::initializer_list<long double> init) {
    if(init.size() != 6) {
        return;
    }

    auto obj_iter  = borders_.begin();
    auto init_iter = init.begin();
    while (obj_iter != borders_.end()) {
        *obj_iter = *init_iter;
        obj_iter++;
        init_iter++;
    }
}