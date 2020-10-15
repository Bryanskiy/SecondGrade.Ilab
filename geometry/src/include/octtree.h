#pragma once

#include <array>
#include <vector>
#include <limits>
#include <cmath>
#include "geometry.h"

namespace octt {

    class space_t {
    public:

        space_t();
        space_t(std::initializer_list<long double> init);

        long double &operator[](std::size_t idx);
        const long double &operator[](std::size_t idx) const;


        void set_Vmin(long double V);
        long double get_Vmin() const;

        long double V() const;

    private:
        std::array<long double, 6> borders_; // borders arr: {xmin, xmax, ymin, ymax, zmin, zmax}
        int V_min_; // V of elementary space area, how to make it static?
    };

    template<typename T>
    space_t create(const std::vector<T>& elements);
}
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

template<typename T>
octt::space_t octt::create(const std::vector<T>& elements) {
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

        ret[4] = std::min(ret[0], proj[2].first);
        ret[5] = std::max(ret[1], proj[2].second);
    }

    ret.set_Vmin(std::pow(ret.V(), 0.33));

    return ret;
}









namespace octt {

    template<typename T>
    class node_t {
    public:
        node_t();
        node_t(const space_t &space);
        ~node_t() = default;

        void print_intersection(std::ostream& out) const;
        void insert(std::size_t i, const T &elem);

        std::size_t children_count() const;

        void del();

    private:
        bool check_elementary(const T& elem);
        bool check_intersection(const T& elem);

        std::array<node_t<T> *, 8> children_;
        std::vector<std::pair<bool,std::pair<std::size_t, T>>> elements_;
        space_t space_;
    };
}

template<typename T>
octt::node_t<T>::node_t() {
    for(std::size_t i = 0, max = children_.size(); i < max; ++i) {
        children_[i] = nullptr;
    }
}

template<typename T>
octt::node_t<T>::node_t(const octt::space_t &space) : node_t() {
    space_ = space;
}

template<typename T>
void octt::node_t<T>::insert(std::size_t i, const T &elem) {
    if(space_.get_Vmin() > space_.V()) {
        bool flag = check_elementary(elem);
        elements_.push_back({flag, {i, elem}});
    }

    std::pair<long double, long double> proj[3];
    proj[0] = elem.projection_i(0);
    proj[1] = elem.projection_i(1);
    proj[2] = elem.projection_i(2);

    long double x_mid = (space_[0] + space_[1]) / 2.0;
    long double y_mid = (space_[2] + space_[3]) / 2.0;
    long double z_mid = (space_[4] + space_[5]) / 2.0;

    std::size_t idx;
    space_t new_space = space_;
    if((proj[0].second < x_mid) && (proj[1].second < y_mid) && (proj[2].second < z_mid)) {
        new_space[1] = x_mid, new_space[3] = y_mid, new_space[5] = z_mid;
        idx = 0;
    } else if((proj[0].second < x_mid) && (proj[1].second < y_mid) && (proj[2].first > z_mid))  {
        new_space[1] = x_mid, new_space[3] = y_mid, new_space[4] = z_mid;
        idx = 1;
    } else if((proj[0].second < x_mid) && (proj[1].first > y_mid) && (proj[2].second < z_mid)) {
        new_space[1] = x_mid, new_space[2] = y_mid, new_space[5] = z_mid;
        idx = 2;
    } else if((proj[0].second < x_mid) && (proj[1].first > y_mid) && (proj[2].first > z_mid)) {
        new_space[1] = x_mid, new_space[2] = y_mid, new_space[4] = z_mid;
        idx = 3;
    } else if((proj[0].first > x_mid) && (proj[1].second < y_mid) && (proj[2].second < z_mid)) {
        new_space[0] = x_mid, new_space[3] = y_mid, new_space[5] = z_mid;
        idx = 4;
    } else if((proj[0].first > x_mid) && (proj[1].second < y_mid) && (proj[2].first > z_mid))  {
        new_space[0] = x_mid, new_space[3] = y_mid, new_space[4] = z_mid;
        idx = 5;
    } else if((proj[0].first > x_mid) && (proj[1].first > y_mid) && (proj[2].second < z_mid)) {
        new_space[0] = x_mid, new_space[2] = y_mid, new_space[5] = z_mid;
        idx = 6;
    } else if((proj[0].first > x_mid) && (proj[1].first > y_mid) && (proj[2].first > z_mid)) {
        new_space[0] = x_mid, new_space[2] = y_mid, new_space[4] = z_mid;
        idx = 7;
    } else {
        bool flag = check_intersection(elem);
        elements_.push_back({flag, {i, elem}});
        return;
    }

    if(children_[idx] == nullptr) {
        children_[idx] = new node_t<T>(new_space);
    }

    children_[idx]->insert(i, elem);
}

template<typename T>
void octt::node_t<T>::print_intersection(std::ostream& out) const {
    for(std::size_t i = 0, max = elements_.size(); i < max; ++i) {
        if(elements_[i].first == true) {
            out << elements_[i].second.first << std::endl;
        }
    }

    for(std::size_t i = 0, max = children_.size(); i < max; ++i) {
        if(children_[i] != nullptr) {
            children_[i]->print_intersection(out);
        }
    }
}

template<typename T>
bool octt::node_t<T>::check_elementary(const T& elem){
    if(elements_.size() == 0) {
        return false;
    }

    bool flag = false;
    for(std::size_t i = 0, max = elements_.size(); i < max; ++i) {
        if(ivkg::intersection(elements_[i].second.second, elem)) {
            elements_[i].first = true;
            flag = true;
        }
    }

    return flag;
}

template<typename T>
bool octt::node_t<T>::check_intersection(const T &elem){
    if(space_.get_Vmin() > space_.V()) {
        return check_elementary(elem);
    }

    bool flag = false;
    for(std::size_t i = 0, max = elements_.size(); i < max; ++i) {
        if(ivkg::intersection(elements_[i].second.second, elem)) {
            elements_[i].first = true;
            flag = true;
        }
    }

    for(std::size_t i = 0, max = children_.size(); i < max; ++i) {
        if(children_[i] != nullptr) {
            flag = flag || children_[i]->check_intersection(elem);
        }
    }

    return flag;
}

template<typename T>
void octt::node_t<T>::del() {
    for(std::size_t i = 0, max = children_.size(); i < max; ++i) {
        if(children_[i] != nullptr) {
            children_[i]->del();
            delete children_[i];
        }
    }
}

template<typename T>
std::size_t octt::node_t<T>::children_count() const{
    std::size_t ret = 0;

    for(std::size_t i = 0, max = children_.size(); i < max; ++i) {
        if(children_[i] != nullptr) {
            ret += 1;
        }
    }

    return ret;
}


namespace octt {
    template<typename T>
    class octtree_t {
    public:
        octtree_t();
        octtree_t(const space_t& space);
        ~octtree_t();

        void print_intersections(std::ostream& out) const;

        void insert(std::size_t i, const T &elem);

    private:
        node_t<T> *root;
    };
}

template<typename T>
octt::octtree_t<T>::octtree_t() {
    root = new node_t<T>;
}

template<typename T>
octt::octtree_t<T>::octtree_t(const octt::space_t& space) {
    root = new node_t<T>(space);
}

template<typename T>
octt::octtree_t<T>::~octtree_t() {
    root->del();
    delete root;
}

template<typename T>
void octt::octtree_t<T>::print_intersections(std::ostream& out) const {
    root->print_intersection(out);
}

template<typename T>
void octt::octtree_t<T>::insert(std::size_t i, const T &elem) {
    root->insert(i, elem);
}



