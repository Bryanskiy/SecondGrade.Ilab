#pragma once

#include <string>
#include <unordered_map>

#include "Inode.hpp"

namespace Inode {

class symtab_local_t final {
    std::unordered_map<std::string, Inode_t*> symbols_;
public:
    void add(const std::string& symbol, Inode_t* node) {
        symbols_[symbol] = node;
    }

    Inode_t* find(const std::string& symbol) {
        auto it = symbols_.find(symbol);
        if(it == symbols_.end()) {
            return nullptr;
        }

        return it->second;
    }

    void free() {
        for(auto& pair: symbols_) {
            delete pair.second;
        }

        symbols_.clear();
    }
};

class symtab_t final {
    std::unordered_map<Iscope_t*, symtab_local_t> tables_;
public:
    void add(Iscope_t* s, std::string symbol, Inode_t* n) {
        auto& tab = tables_[s];
        tab.add(symbol, n);
    }

    Inode_t* exists(Iscope_t * const  s, std::string symbol) {
        auto& tab = tables_[s];
        return tab.find(symbol);
    }

    void free(Iscope_t* s){
        auto& tab = tables_[s];
        tab.free();
    };
};

}