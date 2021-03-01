#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>
	

namespace kgraph {

template<typename VT, typename ET>
class kgraph_t final {
public:
    kgraph_t() : vertex_size_(0u), vertex_capacity_(2u) {graph_.resize(vertex_capacity_);}

    /* retern value - vertex id */
    std::size_t push_vertex(const VT& vertex_data = VT{});
    /* v1, v2 - vertices id */
    void push_edge(std::size_t v1, std::size_t v2, const ET& edge_data = ET{});
    void dump(std::ostream& out) const;

private:
    void vertex_realloc(std::size_t new_vertex_capacity);
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
};

template<typename VT, typename ET>
std::size_t kgraph_t<VT, ET>::push_vertex(const VT& vertex_data /* = VT{} */) {
    if(vertex_size_ == vertex_capacity_) {
        vertex_realloc(vertex_capacity_ * 2);
    }

    graph_[vertex_size_] = {0, vertex_size_, vertex_size_};
    ++vertex_size_;
#ifdef DEBUG
    dump(std::cout);
#endif    
    return vertex_size_;
}

template<typename VT, typename ET>
void kgraph_t<VT, ET>::push_edge(std::size_t v1, std::size_t v2, const ET& edge_data /* = ET{} */ ) {
    if((v1 > vertex_size_) || (v2 > vertex_size_)) {
        throw std::runtime_error("Vertex didn't exist");
    }

    /* first essesnse */
    {
        std::size_t last_essense = graph_[v1 - 1].prev;
        graph_[v1 - 1].prev = graph_.size();
        graph_[last_essense].next = graph_.size();
        graph_.push_back({v1, v1 - 1, last_essense});
    }

    /* second essesnse */
    {
        std::size_t last_essense = graph_[v2 - 1].prev;
        graph_[v2 - 1].prev = graph_.size();
        graph_[last_essense].next = graph_.size();
        graph_.push_back({v2, v2 - 1, last_essense});
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
        if((i >= vertex_size_) && (i < vertex_capacity_)) {continue;}
        if(i == vertex_size_) {out << "| ";}
        out << std::setw(4) << i << " ";
    }
    out << std::endl;

    for(std::size_t i = 0, maxi = graph_.size(); i < maxi; ++i) {
        if((i >= vertex_size_) && (i < vertex_capacity_)) {continue;}
        if(i == vertex_size_) {out << "| ";}
        out << std::setw(4) << graph_[i].incident_vertex << " ";
    }
    out << std::endl;

    for(std::size_t i = 0, maxi = graph_.size(); i < maxi; ++i) {
        if((i >= vertex_size_) && (i < vertex_capacity_)) {continue;}
        if(i == vertex_size_) {out << "| ";}
        out << std::setw(4) << graph_[i].next << " ";
    }
    out << std::endl;

    for(std::size_t i = 0, maxi = graph_.size(); i < maxi; ++i) {
        if((i >= vertex_size_) && (i < vertex_capacity_)) {continue;}
        if(i == vertex_size_) {out << "| ";}
        out << std::setw(4) << graph_[i].prev << " ";
    }
    out << std::endl;
}

} /* namespace kgraph */