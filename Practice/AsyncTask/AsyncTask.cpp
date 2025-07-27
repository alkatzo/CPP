// AsyncTask.cpp : Source file for your target.
//

#include "AsyncTask.h"

#include <vector>
#include <cmath>

#include "threadpool.h"

bool is_prime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;  // 2 and 3 are prime

    if (n % 2 == 0 || n % 3 == 0) return false;

    for (int i = 5; i <= std::sqrt(n); i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

int count_primes(std::vector<int>::iterator begin, std::vector<int>::iterator end) {
    int count = 0;
    while(begin != end) {
        count += static_cast<int>(is_prime(*begin));
        begin++;
    }
    return count;
}


int main()
{
    ThreadPool pool(4);

    constexpr int n = 1000;
    std::vector<int> numbers(n);

    for(int i = 0; i < n; i++) {
        numbers[i] = i;
    }

    size_t splits = 4;
    std::vector<std::future<int>> futures;

    std::vector<int>::iterator start = numbers.begin();
    for(int i = 0; i < splits; i++) {
        auto end = start + numbers.size()/splits;
        futures.push_back(std::move(pool.enqueue(&count_primes, start, end)));
        start = end;
    }

    int prime_count = 0;
    for(auto &f : futures) {
        int count = f.get();
        std::cout << count << std::endl;
        prime_count += count;
    }

    std::cout << "total primes found: " << prime_count << std::endl;

	return 0;
}
