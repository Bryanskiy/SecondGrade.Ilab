#include <iostream>
#include "LFU.h"

struct page_t {
    int id;
};

int main() {
    size_t cache_size, request_count;
    std::cin >> cache_size >> request_count;
    cache_t<page_t> cache(cache_size);
    size_t hits = 0;
    for (int i = 0; i < request_count; ++i) {
        page_t p;
        std::cin >> p.id;
        if (cache.lookup(&p)) {
            hits += 1;
        }
    }
    std::cout << hits << "\n";
}
