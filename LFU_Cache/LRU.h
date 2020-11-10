#pragma once

#include <list>
#include <unordered_map>

template <typename T, typename KeyT = int>
class LRU_t {
public:
    LRU_t(size_t capacity);
    bool lookup(const T& elem);
private:
    size_t capacity_;
    std::list<T> cache_;
    std::unordered_map<KeyT, typename std::list<T>::iterator> htable_;

    bool is_full() const;
};

template <typename T, typename KeyT>
LRU_t<T, KeyT>::LRU_t(size_t capacity) : capacity_(capacity) {}

template <typename T, typename KeyT>
bool LRU_t<T, KeyT>::lookup(const T& elem) {
    /*
     * check if elem in a cache
     */
    auto hit = htable_.find(elem.get_id());

    /*
     * miss in the cache
     */
    if (hit == htable_.end()) {
        if (is_full()) {
            htable_.erase(cache_.back().get_id());
            cache_.pop_back();
        }

        cache_.push_front(elem);
        htable_[elem.get_id()] = cache_.begin();
        return false;
    }

    auto eltit = hit->second;
    if (eltit != cache_.begin())
        cache_.splice(cache_.begin(), cache_, eltit, std::next(eltit));
    return true;
}

template <typename T, typename KeyT>
bool LRU_t<T, KeyT>::is_full() const {
    return cache_.size() == capacity_;
}
