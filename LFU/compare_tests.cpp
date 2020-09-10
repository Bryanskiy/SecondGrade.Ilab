#include <fstream>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <random>
#include <string>

#include "LFU.h"
#include "LRU.h"

class page_t {
public:
    page_t(int id) : id_(id) {}
    int get_id() const {return id_;}
private:
    int id_;
};

/*
 * class for checking the running time of the program
 */
class Timer_t {
public:
    using clock_t = std::chrono::high_resolution_clock;
    using milliseconds_t = std::chrono::milliseconds;

    Timer_t() : start_(clock_t::now()) {}
    milliseconds_t get_time() {
        return std::chrono::duration_cast<milliseconds_t>(clock_t::now() - start_);
    }
    void reset() {
        start_ = clock_t::now();
    }
private:
    std::chrono::time_point<clock_t> start_;
};

/*
 * struct for storage of working time and hit rate
 */
struct stat_t {
    std::chrono::milliseconds time;
    std::size_t hits = 0;
};

/*
 * get statistic of cache algorithm
 * generate random nums from distribution
 * it use std::mt19937 generator
 */
template<typename C, typename D>
stat_t cache_statistic(C& cache, std::size_t request_count, D& distribution) {
    stat_t stat;
    std::mt19937 gen;
    Timer_t timer;

    for(std::size_t i = 0; i < request_count; ++i) {
        int random = distribution(gen);
        page_t page(random);
        stat.hits += cache.lookup(page);
    }

    stat.time = timer.get_time();
    return stat;
}

/*
template<template<typename> class C, typename D>
void write_statistic(char const * const file_name, D& distribution) {
    std::size_t request_count = 1000000;

    std::ofstream out;
    out.open(file_name);

    for(std::size_t i = 10; i <= 20; ++i) {
        C<page_t> cache_(i);
        out << i << ' ';
        stat_t stat = cache_stat(cache_, request_count, distribution);
        out << stat.hits << ' ' << stat.time.count() << std::endl;
    }

    out.close();
}

std::uniform_int_distribution<> dist(0, 1000);
write_statistic<LFU_t>("LFU_uniform_int_distribution.txt", dist); ????????????????????????????
*/

void collect_statistic() {
    std::size_t request_count = 1000000;

    {
        std::uniform_int_distribution<> distribution(0, 1000);
        std::ofstream out;
        out.open("statistic/LFU_uniform_int_distribution.txt");

        for(std::size_t i = 10; i <= 20; ++i) {
            LFU_t<page_t> cache(i);
            out << i << ' ';
            stat_t stat = cache_statistic(cache, request_count, distribution);
            out << stat.hits << ' ' << stat.time.count() << std::endl;
        }

        out.close();
    }

    {
        std::uniform_int_distribution<> distribution(0, 1000);
        std::ofstream out;
        out.open("statistic/LRU_uniform_int_distribution.txt");

        for(std::size_t i = 10; i <= 20; ++i) {
            LRU_t<page_t> cache(i);
            out << i << ' ';
            stat_t stat = cache_statistic(cache, request_count, distribution);
            out << stat.hits << ' ' << stat.time.count() << std::endl;
        }

        out.close();
    }
}

int main() {
    collect_statistic();
}