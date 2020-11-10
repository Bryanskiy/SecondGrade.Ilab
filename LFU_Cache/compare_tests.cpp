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

template<typename C, typename D>
void write_statistic(char const * const file_name, std::size_t request_count, D& distribution)  {
    std::ofstream out;
    out.open(file_name);

    for(std::size_t i = 10; i <= 200; ++i) {
        C cache(i);
        out << i << ' ';
        stat_t stat = cache_statistic(cache, request_count, distribution);
        out << stat.hits << ' ' << stat.time.count() << std::endl;
    }

    out.close();
}

void collect_statistic() {
    std::size_t request_count = 100000;
    {
        std::uniform_int_distribution<> dist(0, 1000);
        write_statistic<LFU_t<page_t>>("statistic/LFU_uniform_int_distribution.txt", request_count, dist);
    }

    {
        std::uniform_int_distribution<> dist(0, 1000);
        write_statistic<LRU_t<page_t>>("statistic/LRU_uniform_int_distribution.txt", request_count, dist);
    }
}

int main() {
    collect_statistic();
}