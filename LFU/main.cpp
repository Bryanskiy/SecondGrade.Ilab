#include <iostream>

#include "LFU.h"
#include "unit_tests.h"

class page_t {
public:
    page_t(int id) : id_(id) {}
    int get_id() const {return id_;}
private:
    int id_;
};

void test1() {
    /*
     * cache_size = 3
     * pages: 7 0 1 2 0 3 0 4 2 3 0 3 2 1 2
     */
    LFU_t<page_t> cache(3);
    AssertEqual(cache.lookup(page_t(7)), false, "id = 7");
    AssertEqual(cache.lookup(page_t(0)), false, "id = 0");
    AssertEqual(cache.lookup(page_t(1)), false, "id = 1");
    AssertEqual(cache.lookup(page_t(2)), false, "id = 2");
    AssertEqual(cache.lookup(page_t(0)), true, "id = 0");
    AssertEqual(cache.lookup(page_t(3)), false, "id = 3");
    AssertEqual(cache.lookup(page_t(0)), true, "id = 0");
    AssertEqual(cache.lookup(page_t(4)), false, "id = 4");
    AssertEqual(cache.lookup(page_t(2)), false, "id = 2");
    AssertEqual(cache.lookup(page_t(3)), false, "id = 3");
    AssertEqual(cache.lookup(page_t(0)), true, "id = 0");
    AssertEqual(cache.lookup(page_t(3)), true, "id = 3");
    AssertEqual(cache.lookup(page_t(2)), true, "id = 2");
    AssertEqual(cache.lookup(page_t(1)), false, "id = 1");
    AssertEqual(cache.lookup(page_t(2)), true, "id = 2");
}

void test_all() {
    test_runner_t tr;
    tr.run_test(test1, "test1");
}

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