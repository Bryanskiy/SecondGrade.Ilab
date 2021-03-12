#pragma once

#include <string>
#include <unordered_map>

#include "Inode.hpp"

namespace Inode {

class symtab_local_t final {
    std::unordered_map<std::string, Inode_t*> symbols_;
public:
    symtab_local_t() = default;
    symtab_local_t(const symtab_local_t&) = delete;
    symtab_local_t& operator=(const symtab_local_t&) = delete;

    void add(const std::string& symbol, Inode_t* node) {
        symbols_[symbol] = node;
    }

    Inode_t* find(const std::string& symbol) const {
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
    symtab_t() = default;
    symtab_t(const symtab_t&) = delete;
    symtab_t& operator=(const symtab_t&) = delete;

    void add(Iscope_t* s, std::string symbol, Inode_t* n) {
        auto& tab = tables_[s];
        tab.add(symbol, n);
    }

    Inode_t* exists(Iscope_t * const  s, const std::string& symbol) {
        /* perform insertion if key doesn't exist */
        auto& tab = tables_[s];
        return tab.find(symbol);
    }

    void free(Iscope_t* s){
        auto& tab = tables_[s];
        tab.free();
    };
};

}