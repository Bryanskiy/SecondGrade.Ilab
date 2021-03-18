#pragma once

#include <array>
#include <vector>
#include <limits>
#include <cmath>
#include <algorithm>
#include "geometry.h"
#include "queue"

namespace octt {

    class space_t {
    public:

        space_t();
        space_t(std::initializer_list<long double> init);

        long double &operator[](std::size_t idx) { return borders_[idx]; }
        const long double &operator[](std::size_t idx) const { return borders_[idx];}

    private:
        std::array<long double, 6> borders_; // borders arr: {xmin, xmax, ymin, ymax, zmin, zmax}
    };
}



namespace octt {

    template<typename T>
    class node_t {
    public:
        node_t();
        node_t(const space_t &space, uint8_t height);
        ~node_t() = default;

        void insert(T* elem);
        void fill_intersection();    
        std::size_t children_count() const;
        void del();

    private:

        static constexpr uint8_t max_height_ = 3;

        void fill_collision(T* elem);

        std::array<node_t<T>*, 8> children_;
        std::vector<T*> elements_;
        space_t space_;
        uint8_t height_;
    };
}

template<typename T>
octt::node_t<T>::node_t() {
    for(std::size_t i = 0, max = children_.size(); i < max; ++i) {
        children_[i] = nullptr;
    }

    height_ = 0;
}

template<typename T>
octt::node_t<T>::node_t(const octt::space_t& space, uint8_t height) : node_t() {
    space_ = space;
    height_ = height;
}

template<typename T>
void octt::node_t<T>::insert(T* elem) {
    if(height_ >= max_height_) {
        elements_.push_back(elem);
        return;
    }

    std::pair<long double, long double> proj[3];
    proj[0] = elem->projection_i(0);
    proj[1] = elem->projection_i(1);
    proj[2] = elem->projection_i(2);

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
        elements_.push_back(elem);
        return;
    }

    if(children_[idx] == nullptr) {
        children_[idx] = new node_t<T>(new_space, height_ + 1);
    }

    children_[idx]->insert(elem);
}

template<typename T>
void octt::node_t<T>::fill_intersection() {
    for(std::size_t i = 0, maxi = elements_.size(); i < maxi; ++i) {
        for(std::size_t j = i + 1, maxj = maxi; j < maxj; ++j) {
            if(lingeo::intersection(elements_[i]->getTriangle(), elements_[j]->getTriangle())) {
                elements_[i]->setColor({1.0, 0.0, 0.0});
                elements_[j]->setColor({1.0, 0.0, 0.0});
            }
        }


        for(std::size_t j = 0, maxj = children_.size(); j < maxj; ++j) {
             if(children_[j] != nullptr) {
                children_[j]->fill_collision(elements_[i]);
            }
        }
    }

    for(std::size_t i = 0, maxi = children_.size(); i < maxi; ++i) {
        if(children_[i] != nullptr) {
            children_[i]->fill_intersection();
        }
    }
}

template<typename T>
void octt::node_t<T>::fill_collision(T* elem) {
    for(std::size_t i = 0, maxi = elements_.size(); i < maxi; ++i) {
        if(lingeo::intersection(elem->getTriangle(), elements_[i]->getTriangle())) {
            elements_[i]->setColor({1.0, 0.0, 0.0});
            elem->setColor({1.0, 0.0, 0.0});
        }
    }

    for(std::size_t i = 0, maxi = children_.size(); i < maxi; ++i) {
        if(children_[i] != nullptr) {
            children_[i]->fill_collision(elem);
        }
    }    
}

template<typename T>
void octt::node_t<T>::del() {
    std::queue<node_t<T>*> q;

    for(std::size_t i = 0, max = children_.size(); i < max; ++i) {
        if(children_[i] != nullptr) {
            q.push(children_[i]);
        }
    }

    while(!q.empty()) {
        node_t<T>* node = q.front();
        q.pop();

        for(std::size_t i = 0, max = children_.size(); i < max; ++i) {
            if(node->children_[i] != nullptr) {
                q.push(node->children_[i]);
            }
        }

        delete node;
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
        octtree_t(const space_t& space);
        ~octtree_t();

        void insert(T* elem);
        void fill_intersection();

    private:
        node_t<T> *root;
    };
}

template<typename T>
octt::octtree_t<T>::octtree_t(const octt::space_t& space) {
    root = new node_t<T>(space, 0);
}

template<typename T>
octt::octtree_t<T>::~octtree_t() {
    root->del();
    delete root;
}

template<typename T>
void octt::octtree_t<T>::insert(T* elem) {
    root->insert(elem);
}

template<typename T>
void octt::octtree_t<T>::fill_intersection() {
    root->fill_intersection();
}