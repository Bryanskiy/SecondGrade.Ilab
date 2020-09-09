#pragma once

#include <unordered_map>
#include <map>

/*
 * T - page
 * KeyT - page id
 */
template<typename T, typename KeyT = int>
class LFU_t {
public:
    LFU_t(std::size_t capacity);
    bool is_full() const;
    bool lookup(const T& elem);

private:
    /*
     * todo:
     * solve situation when cap = 0
     */
    std::size_t capacity_;
    std::multimap<size_t, T> cache_;
    std::unordered_map<KeyT, typename std::multimap<size_t, T>::iterator> htable_;
};

template<typename T, typename KeyT>
LFU_t<T, KeyT>::LFU_t(std::size_t capacity) :
    capacity_(capacity),
    cache_(),
    htable_() {}

template<typename T, typename KeyT>
bool LFU_t<T, KeyT>::is_full() const{
    return cache_.size() == capacity_;
}

template<typename T, typename KeyT>
bool LFU_t<T, KeyT>::lookup(const T& elem) {
    /*
     * check if elem in a cache
     */
    auto hit = htable_.find(elem.get_id());
    /*
     * miss in the cache
     */
    if(hit == htable_.end()) {
        if(is_full()) {
            auto it = cache_.begin();
            cache_.erase(it);
            htable_.erase(it->second.get_id());
        }

        auto new_it = cache_.insert(std::make_pair(1, elem));
        htable_[elem.get_id()] = new_it;
        return false;
    }

    /*
     * hit to the cache
     * increment frequency deleting prev page
     */
    size_t new_frec = hit->second->first + 1;

    cache_.erase(hit->second);

    auto new_it = cache_.insert(std::make_pair(new_frec, elem));
    htable_[elem.get_id()] = new_it;
    return true;
}
