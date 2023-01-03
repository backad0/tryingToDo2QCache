#pragma once
#include <iterator>
#include <list>
#include <unordered_map>
#include <vector>

namespace caches{
    template <typename T, typename KeyT = int> struct cache_t{
        size_t sz_;
        int sz_1;
        std::list<std::pair<KeyT, T>> cache_;
        std::list<std::pair<KeyT, T>> sub_cache_;

        using ListIt = typename std::list<std::pair<KeyT, T>>::iterator;
        using SubListIt = typename std::list<std::pair<KeyT, T>>::iterator;
        std::unordered_map<KeyT, ListIt> hash_;
        std::unordered_map<KeyT, SubListIt> hash_1;

        cache_t(size_t sz, int sz1): sz_(sz), sz_1(sz1) {}

        bool full() const { return (cache_.size() == sz_);}
        bool subFull() const { return (sub_cache_.size() == sz_1);}

        template <typename F> bool lookup_update(KeyT key, F slow_get_page) {
            auto hit1 = hash_1.find(key);
            if (hit1 == hash_1.end()){
                if (subFull()) {
                    hash_1.erase(sub_cache_.back().first);
                    sub_cache_.pop_back();
                }
                sub_cache_.emplace_front(key, slow_get_page(key));
                hash_1.emplace(key,sub_cache_.begin());
                return false;
            }

            auto hit = hash_.find(key);
            if (hit == hash_.end()){
                if (full()) {
                    hash_.erase(cache_.back().first);
                    cache_.pop_back();
                }
                cache_.emplace_front(key, slow_get_page(key));
                hash_.emplace(key,cache_.begin());
                return false;
            }
            auto eltit = hit->second;
            if (eltit != cache_.begin())
                cache_.splice(cache_.begin(), cache_, eltit, std::next(eltit));
            return true;
        }
    };
}