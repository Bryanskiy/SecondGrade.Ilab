#pragma once

#include <list>
#include <unordered_map>

#ifdef __CACHE_DEBUG__
#include <iostream>
#endif

template <typename T, typename KeyT = int>
class cache_t {
public:
#ifdef __CACHE_DEBUG__
    void print(std::ostream& out);
#endif

    explicit cache_t(size_t capacity);
    bool lookup(const T& elem);
private:
    size_t capacity_;
    std::list<std::pair<T, size_t>> cache_;
    std::unordered_map<KeyT, typename std::list<std::pair<T, size_t>>::iterator> hash_;

    bool is_full() const;
};

#ifdef __CACHE_DEBUG__
template <typename T, typename KeyT>
void cache_t<T, KeyT>::print(std::ostream &out) {
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
cache_t<T, KeyT>::cache_t(size_t capacity) : capacity_(capacity){}

template <typename T, typename KeyT>
bool cache_t<T, KeyT>::lookup(const T& elem) {
    auto hit = hash_.find(elem.get_id());
    if (hit == hash_.end()) {
        if (is_full()) {

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

    ++(hit->second->second);
    return true;
}

template <typename T, typename KeyT>
bool cache_t<T, KeyT>::is_full() const {
    return cache_.size() == capacity_;
}
