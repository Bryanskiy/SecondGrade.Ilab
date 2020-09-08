#pragma once

#include <list>
#include <unordered_map>
#include <chrono>
#include <thread>

#ifdef __CACHE_DEBUG__
#include <iostream>
#endif

template <typename T, typename KeyT = int>
class LFU_t {
public:
#ifdef __CACHE_DEBUG__
    void print(std::ostream& out);
#endif

    explicit LFU_t(size_t capacity);
    bool lookup(const T& elem);
private:
    size_t capacity_;
    std::list<std::pair<T, size_t>> cache_;
    std::unordered_map<KeyT, typename std::list<std::pair<T, size_t>>::iterator> hash_;

    bool is_full() const;

    /*
     * gets access to the page if it is in the cache/not in the cache
     * since this is an emulator functions are a delay
     */
    void local_access();
    void net_access();
};

/*
 * print information for debug
*/
#ifdef __CACHE_DEBUG__
template <typename T, typename KeyT>
void LFU_t<T, KeyT>::print(std::ostream &out) {
    auto it = begin(cache_);
    if(it != end(cache_)) {
        out << '(' << it->first.id << ", " << it->second << ')';
        ++it;
    }
    for(;it != end(cache_); ++it) {
        out << ' ' <<'(' << it->first.id << ", " << it->second << ')';
    }
}
#endif

template <typename T, typename KeyT>
LFU_t<T, KeyT>::LFU_t(size_t capacity) : capacity_(capacity) {}

template <typename T, typename KeyT>
bool LFU_t<T, KeyT>::lookup(const T& elem) {
    /*
     * check if elem in a cache
     */
    auto hit = hash_.find(elem.get_id());

    /*
     * miss in the cache
     */
    if (hit == hash_.end()) {
        if (is_full()) {

            net_access();
            /*
             * find elem with min frequency
             */
            auto min_frec = begin(cache_);
            for(auto it = begin(cache_); it != end(cache_); ++it) {
                if(it->second <= min_frec->second) {
                    min_frec = it;
                }
            }

            hash_.erase(min_frec->first.get_id());
            cache_.erase(min_frec);
        }

        cache_.push_front({elem, 1});
        hash_[elem.get_id()] = cache_.begin();

        return false;
    }

    local_access();
    ++(hit->second->second);
    return true;
}

template <typename T, typename KeyT>
bool LFU_t<T, KeyT>::is_full() const {
    return cache_.size() == capacity_;
}

template<typename T, typename KeyT>
void LFU_t<T, KeyT>::local_access() {
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
}

template<typename T, typename KeyT>
void LFU_t<T, KeyT>::net_access() {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}
