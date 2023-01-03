#include <iostream>
#include <cassert>
#include "cache.hpp"

int slow_get_page_int(int key) { return key;}

int main() {
    int hits = 0;
    int n;
    size_t m;
    int sunSize;

    std::cin >> m >> sunSize >> n;
    assert(std::cin.good());
    caches::cache_t<int> c{m,sunSize};

    for (int i = 0; i < n; ++i) {
        int q;
        std::cin >> q;
        assert(std::cin.good());
        if (c.lookup_update(q, slow_get_page_int))
            hits+=1;
    }
    std::cout << hits << std::endl;
}