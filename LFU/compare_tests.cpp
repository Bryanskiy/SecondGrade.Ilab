#include <iostream>
#include <ctime>
#include <cstdlib>

#include "LFU.h"
#include "LRU.h"

class page_t {
public:
    page_t(int id) : id_(id) {}
    int get_id() const {return id_;}
private:
    int id_;
};

int main() {
    LFU_t<page_t> lfu_cache(100);
    LRU_t<page_t> lru_cache(100);
    std::size_t lfu_hits, lru_hits;
    std::srand(std::time(0));
    for(std::size_t i = 0; i < 10000; ++i) {
        int random = rand() % 500;
        page_t page(random);
        lfu_hits += lfu_cache.lookup(page);
        lru_hits += lru_cache.lookup(page);
    }

    std::cout << "LFU hits: " << lfu_hits << std::endl;
    std::cout << "LRU hits: " << lru_hits << std::endl;
}