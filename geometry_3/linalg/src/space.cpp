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

long double& octt::space_t::operator[](std::size_t idx) {
    return borders_[idx];
}

const long double& octt::space_t::operator[](std::size_t idx) const {
    return borders_[idx];
}

long double octt::space_t::V() const {
    return (borders_[1] - borders_[0]) * (borders_[3] - borders_[2]) * (borders_[5] - borders_[4]);
}

void octt::space_t::set_Vmin(long double V) {
    V_min_ = V;
}

long double octt::space_t::get_Vmin() const {
    return V_min_;
}

octt::space_t octt::create(const std::vector<lingeo::triangle_t<3>>& elements) {
    if(!elements.size()) {
        return space_t{};
    }

    std::pair<long double, long double> proj[3];
    proj[0] = elements[0].projection_i(0);
    proj[1] = elements[0].projection_i(1);
    proj[2] = elements[0].projection_i(2);

    space_t ret{proj[0].first, proj[0].second, proj[1].first, proj[1].second, proj[2].first, proj[2].second};
    for(std::size_t i = 1, max = elements.size(); i < max; ++i) {
        proj[0] = elements[i].projection_i(0);
        proj[1] = elements[i].projection_i(1);
        proj[2] = elements[i].projection_i(2);

        ret[0] = std::min(ret[0], proj[0].first);
        ret[1] = std::max(ret[1], proj[0].second);

        ret[2] = std::min(ret[2], proj[1].first);
        ret[3] = std::max(ret[3], proj[1].second);

        ret[4] = std::min(ret[4], proj[2].first);
        ret[5] = std::max(ret[5], proj[2].second);
    }

    ret.set_Vmin(std::pow(ret.V(), 0.33));

    return ret;
}