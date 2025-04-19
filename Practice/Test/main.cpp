#include <iostream>
#include <unordered_map>
#include <list>
#include <memory>
#include <string>
#include <utility>
#include <chrono>
#include <cmath>
#include <numeric>


using uint = unsigned int;

uint modpow(uint b, uint e, uint mod) {
    uint r = 1;

    for(int i = 0; i < e; i++) {
        r = (r * b) % mod;
    }

    return r % mod;
}

// find minimum index of max a[i]^a[i+1] % (10^9 + 7)
int findMinIndex(const std::vector<uint>& arr) {
    int min_idx = -1;
    int max_modulo = -1;

    int modulo = std::pow(10, 9) + 7;

    for(int i = 0; i < arr.size() - 1; i++) {
        int res  = modpow(arr[i], arr[i+1], modulo);
        if(res > max_modulo) {
            max_modulo = res;
            min_idx = i;
        }
    }

    return min_idx;
}


int main()
{
    std::vector<uint> arr{(uint)std::pow(10, 5),
                         (uint)std::pow(10, 2),
                         (uint)std::pow(2, 5),
                         (uint)std::pow(3, 5),
                         (uint)std::pow(4, 5),
                         (uint)std::pow(10, 5),
                         (uint)std::pow(10, 5),
                         (uint)std::pow(10, 5),
                         (uint)std::pow(10, 5),
    };

    std::cout << findMinIndex(arr);

    return 0;
}

