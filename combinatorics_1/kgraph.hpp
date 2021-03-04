#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>
#include <unordered_map>
#include <iomanip>
	

namespace kgraph {

template<typename VT, typename ET>
class kgraph_t final {
public:
    kgraph_t() : vertex_size_(0u), vertex_capacity_(2u) {graph_.resize(vertex_capacity_);}

    /* v1, v2 - vertices id */
    void push_edge(std::size_t v1, std::size_t v2, const ET& edge_data = ET{});
    void dump(std::ostream& out) const;

private:
    void vertex_realloc(std::size_t new_vertex_capacity);
    /*  v1      - user id
        retern  - internal idx */
    std::size_t push_vertex(std::size_t v);
private:
    /* essense can be vertex or edge */
    struct essense_t {
        std::size_t incident_vertex; /* vertex id */
        /* next, prev - idx of appropriate essense */
        std::size_t next;
        std::size_t prev;
    };
    std::vector<essense_t> graph_;

    std::size_t vertex_capacity_;
    std::size_t vertex_size_;

    /* vertex_data_[i] matchs to graph_[i] vertex */
    std::vector<VT> vertex_data_;
    /* edge_data_[i] matchs to graph_[i + vertex_capacity_] edge */
    std::vector<ET> edge_data_;

    /* bijection between iser id's and internal idx for vertices */
    std::unordered_map<std::size_t, std::size_t> user2internal_;
    std::unordered_map<std::size_t, std::size_t> internal2user_;
};


template<typename VT, typename ET>
std::size_t kgraph_t<VT, ET>::push_vertex(std::size_t v) {
    if(user2internal_.find(v) != user2internal_.end()) {
        return user2internal_[v];
    }

    if(vertex_size_ == vertex_capacity_) {
        vertex_realloc(vertex_capacity_ * 2u);
    }

    std::size_t ret = user2internal_[v] = vertex_size_;
    internal2user_[vertex_size_] = v;
    
    graph_[ret] = {0, ret, ret};

    ++vertex_size_;
    return ret;
}

template<typename VT, typename ET>
void kgraph_t<VT, ET>::push_edge(std::size_t v1, std::size_t v2, const ET& edge_data /* = ET{} */ ) {
    std::size_t internal_v1 = push_vertex(v1);
    std::size_t internal_v2 = push_vertex(v2);

    /* first essesnse */
    {
        std::size_t last_essense = graph_[user2internal_[v1]].prev;
        graph_[user2internal_[v1]].prev = graph_.size();
        graph_[last_essense].next = graph_.size();
        graph_.push_back({v1, user2internal_[v1], last_essense});
    }

 #ifdef DEBUG
    dump(std::cout);
#endif     

    /* second essesnse */
    {
        std::size_t last_essense = graph_[user2internal_[v2]].prev;
        graph_[user2internal_[v2]].prev = graph_.size();
        graph_[last_essense].next = graph_.size();
        graph_.push_back({v2, user2internal_[v2], last_essense});
    }

#ifdef DEBUG
    dump(std::cout);
#endif 
      
}

template<typename VT, typename ET>
void kgraph_t<VT, ET>::vertex_realloc(std::size_t new_vertex_capacity) {
    std::size_t cap_delta =  new_vertex_capacity - vertex_capacity_;
    std::vector<essense_t> tmp(graph_.size() + cap_delta);
    for(std::size_t i = 0, maxi = vertex_size_; i < maxi; ++i) {
        tmp[i].incident_vertex = 0u;
        tmp[i].next = graph_[i].next + cap_delta;
        tmp[i].prev = graph_[i].prev + cap_delta;
    }

    for(std::size_t i = vertex_capacity_, maxi = graph_.size(); i < maxi; ++i) {
        tmp[i + cap_delta].incident_vertex = graph_[i].incident_vertex;
        tmp[i + cap_delta].next = (graph_[i].next >= new_vertex_capacity) ? graph_[i].next + cap_delta : graph_[i].next;
        tmp[i + cap_delta].prev = (graph_[i].next >= new_vertex_capacity) ? graph_[i].prev + cap_delta : graph_[i].prev;
    }

    vertex_capacity_ += cap_delta;
    tmp.swap(graph_);
}


template<typename VT, typename ET>
void kgraph_t<VT, ET>::dump(std::ostream& out) const {
    for(std::size_t i = 0, maxi = graph_.size(); i < maxi; ++i) {
        if(i == vertex_size_) {out << "| ";}
        if((i >= vertex_size_) && (i < vertex_capacity_)) {continue;}
        out << std::setw(4) << i << " ";
    }
    out << std::endl;

    for(std::size_t i = 0, maxi = graph_.size(); i < maxi; ++i) {
        if(i == vertex_size_) {out << "| ";}
        if((i >= vertex_size_) && (i < vertex_capacity_)) {continue;}
        out << std::setw(4) << graph_[i].incident_vertex << " ";
    }
    out << std::endl;

    for(std::size_t i = 0, maxi = graph_.size(); i < maxi; ++i) {
        if(i == vertex_size_) {out << "| ";}
        if((i >= vertex_size_) && (i < vertex_capacity_)) {continue;}
        out << std::setw(4) << graph_[i].next << " ";
    }
    out << std::endl;

    for(std::size_t i = 0, maxi = graph_.size(); i < maxi; ++i) {
        if(i == vertex_size_) {out << "| ";}
        if((i >= vertex_size_) && (i < vertex_capacity_)) {continue;}
        out << std::setw(4) << graph_[i].prev << " ";
    }
    out << std::endl;
}

} /* namespace kgraph */