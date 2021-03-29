#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>
#include <unordered_map>
#include <iomanip>
#include <stack>	
#include <cassert>
#include <optional>

namespace kgraph {

struct color_t {
    enum COLOR {
        blue,
        red,
        empty,
    };

    static COLOR get_another(COLOR color) {
        if(color == empty) {return empty;}
        return (color == blue) ? red : blue;
    }

    static const char* get_string(COLOR color) {
        switch(color) {
            case blue:  return "b";
            case red:   return "r";
            case empty: return "empty";
        }

        assert(0);
        return "";
    }
};

template<typename VT, typename ET>
class kgraph_t final {
public:
    kgraph_t() : vertex_size_(0u), vertex_capacity_(1u) {
        graph_.resize(vertex_capacity_);
        vertex_data_.resize(vertex_capacity_);
    }

    /* v1, v2 - user vertices idx */
    void push_edge(std::size_t v1, std::size_t v2, const ET& edge_data = ET{});
    void dump(std::ostream& out) const;

    /* v - iser vertex id 
       return cycle of odd len, if it exists */
    std::optional<std::vector<std::size_t>> fill_bipartite_color(std::size_t v, color_t::COLOR v_color);

    /* return vector of pair's of user idx and color */
    std::vector<std::pair<std::size_t, color_t::COLOR>> get_color() const;

private:
    void vertex_realloc(std::size_t new_vertex_capacity);
    /*  v1      - user idx
        retern  - internal idx */
    std::size_t push_vertex(std::size_t v);

    /* return internal idx */
    std::size_t pair_incident_vertex(std::size_t edge_id);

    /* v - internal id */
    bool fill_bipartite_itirate(std::size_t w, std::vector<std::size_t>& odd_cycle);
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
    struct vertex_data_t {
        VT data;
        color_t::COLOR color = color_t::empty;
    };
    std::vector<vertex_data_t> vertex_data_;
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
        std::size_t last_essense = graph_[internal_v1].prev;
        graph_[internal_v1].prev = graph_.size();
        graph_[last_essense].next = graph_.size();
        graph_.push_back({internal_v1, internal_v1, last_essense});
    }

    /* second essesnse */
    if(internal_v1 != internal_v2) {
        std::size_t last_essense = graph_[internal_v2].prev;
        graph_[internal_v2].prev = graph_.size();
        graph_[last_essense].next = graph_.size();
        graph_.push_back({internal_v2, internal_v2, last_essense});
    }
}

template<typename VT, typename ET>
void kgraph_t<VT, ET>::vertex_realloc(std::size_t new_vertex_capacity) {
    std::size_t cap_delta =  new_vertex_capacity - vertex_capacity_;
    std::vector<essense_t> tmp(graph_.size() + cap_delta);
    for(std::size_t i = 0, maxi = vertex_size_; i < maxi; ++i) {
        tmp[i].incident_vertex = 0u;
        tmp[i].next = (graph_[i].next >= vertex_capacity_) ? graph_[i].next + cap_delta : graph_[i].next;
        tmp[i].prev = (graph_[i].next >= vertex_capacity_) ? graph_[i].prev + cap_delta : graph_[i].prev;
    }

    for(std::size_t i = vertex_capacity_, maxi = graph_.size(); i < maxi; ++i) {
        tmp[i + cap_delta].incident_vertex = graph_[i].incident_vertex;
        tmp[i + cap_delta].next = (graph_[i].next >= vertex_capacity_) ? graph_[i].next + cap_delta : graph_[i].next;
        tmp[i + cap_delta].prev = (graph_[i].next >= vertex_capacity_) ? graph_[i].prev + cap_delta : graph_[i].prev;
    }

    vertex_capacity_ += cap_delta;
    vertex_data_.resize(vertex_capacity_);
    tmp.swap(graph_);
}

template<typename VT, typename ET>
std::optional<std::vector<std::size_t>> kgraph_t<VT, ET>::fill_bipartite_color(std::size_t v, color_t::COLOR v_color) {
    std::size_t ret = true;
    std::size_t start_v = vertex_capacity_ + 1;
    std::vector<std::size_t> odd_cycle;

    for(std::size_t w = 0; w < vertex_size_; ++w) {
        if(internal2user_[w] == v) {
            start_v = w;
            vertex_data_[w].color = v_color;
            ret = fill_bipartite_itirate(w, odd_cycle);
            break;
        }
    }

    if(start_v > vertex_capacity_) {
        throw std::runtime_error("invalid argument in fill_bipartite_color");
    }

    for(std::size_t w = 0; (w < vertex_size_) && ret; ++w) {
        if(vertex_data_[w].color != color_t::empty) {
            continue;
        }

        if(w == start_v) {
            continue;
        }

        vertex_data_[w].color = v_color;
        ret = fill_bipartite_itirate(w, odd_cycle);
    }

    return (ret) ? std::optional<std::vector<std::size_t>>() : odd_cycle;
}

template<typename VT, typename ET>
bool kgraph_t<VT, ET>::fill_bipartite_itirate(std::size_t w, std::vector<std::size_t>& odd_cycle) {
    std::stack<std::size_t> stack;
    std::vector<size_t> parents(vertex_size_);

    stack.push(w);

    /* B4 - B8 loop */
    while(!stack.empty()) {
        std::size_t current_vertex = stack.top();
        color_t::COLOR current_vertex_color = vertex_data_[current_vertex].color;
        stack.pop();

        std::size_t current_edge = graph_[current_vertex].next;

        /* B5 - B7 loop */
        while(current_edge != current_vertex) {
            std::size_t tmp_vertex = pair_incident_vertex(current_edge);

            if(tmp_vertex == current_vertex) {
                odd_cycle.push_back(internal2user_[tmp_vertex]);
                return false;
            }

            color_t::COLOR tmp_vertex_color = vertex_data_[tmp_vertex].color;

            if(tmp_vertex_color == color_t::empty) {
                vertex_data_[tmp_vertex].color = color_t::get_another(current_vertex_color);
                stack.push(tmp_vertex);
                parents[tmp_vertex] = current_vertex;
            } else if(tmp_vertex_color == current_vertex_color) {

                std::size_t p = parents[current_vertex];
                
                odd_cycle.push_back(internal2user_[current_vertex]);
                while(p != parents[tmp_vertex]) {
                    odd_cycle.push_back(internal2user_[p]);
                    p = parents[p];
                }

                odd_cycle.push_back(internal2user_[p]);
                odd_cycle.push_back(internal2user_[tmp_vertex]);

                return false;
            }

            current_edge = graph_[current_edge].next;
        }
    }

    return true;
}

template<typename VT, typename ET>
std::vector<std::pair<std::size_t, color_t::COLOR>> kgraph_t<VT, ET>::get_color() const {
    std::vector<std::pair<std::size_t, color_t::COLOR>> ans;
    ans.reserve(vertex_size_);

    for(std::size_t i = 0; i < vertex_size_; ++i) {
        color_t::COLOR v_color = vertex_data_[i].color;
        std::size_t user_idx = internal2user_.at(i);
        ans.push_back({user_idx, v_color});
    }

    return ans;
}

template<typename VT, typename ET>
std::size_t kgraph_t<VT, ET>::pair_incident_vertex(std::size_t edge_id) {
    return ((edge_id) % 2) ? graph_[edge_id - 1].incident_vertex : graph_[edge_id + 1].incident_vertex;
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
        if(i < vertex_size_) {
            out << std::setw(4) << graph_[i].incident_vertex << " ";
        } else {
            out << std::setw(4) << internal2user_.at(graph_[i].incident_vertex) << " ";
        }    
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