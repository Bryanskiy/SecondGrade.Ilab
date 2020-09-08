#include "unit_tests.h"

void Test1() {
    /*
     * cache_size = 3
     * pages: 7 0 1 2 0 3 0 4 2 3 0 3 2 1 2
     */

    cache_t<page_t> cache(3);
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
    AssertEqual(cache.lookup(page_t(2)), false, "id = 2");
}

void TestAll() {
    TestRunner tr;
    tr.RunTest(Test1, "Test1");
}

