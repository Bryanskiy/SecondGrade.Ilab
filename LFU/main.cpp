#include <iostream>

#include "LFU.h"

class page_t {
public:
    page_t(int id) : id_(id) {}
    int get_id() const {return id_;}
private:
    int id_;
};

int main() {
    std::size_t cache_size, request_count;
    std::cin >> cache_size >> request_count;
    LFU_t<page_t> cache(cache_size);

    int hits = 0;
    for (int i = 0; i < request_count; ++i) {
        int id; std::cin >> id;
        page_t page(id);
        if (cache.lookup(page))
            hits += 1;
    }
    std::cout << hits << "\n";
}