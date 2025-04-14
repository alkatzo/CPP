#include <iostream>
#include <thread>
#include <vector>
#include <numeric>
#include <string>


#define MIN_BLOCK_SIZE 1000

using namespace std;


void accumulate_example() {
    std::vector<int> v{1,2,3,4,5,6,7,8,9,10};

    int sum = std::accumulate(v.begin(), v.end(), 0);

    int prod = std::accumulate(v.begin(), v.end(), 1, std::multiplies<int>());

    auto dash_fold = [](std::string str, int i){
        return std::move(str) + "-" + std::to_string(i);
    };

    std::string res = std::accumulate(std::next(v.begin()), v.end(), std::to_string(v[0]), std::move(dash_fold));

    std::cout << sum << std::endl;
    std::cout << prod << std::endl;
    std::cout << res << std::endl;

}

template<typename T, typename iterator>
T parallel_accumulate(iterator start, iterator end) {
    size_t input_size = std::distance(start, end);
    int n_blocks = input_size / MIN_BLOCK_SIZE + (input_size % 1000 > 0);

    std::vector<int> block_sums(n_blocks);

    iterator block_start = start;
    for(int i = 0; i < n_blocks; i++) {
        iterator block_end = block_start + MIN_BLOCK_SIZE;
        block_sums[i] = std::accumulate(block_start, block_end, 0);
        block_start = block_end;
    }

    return std::accumulate(block_sums.begin(), block_sums.end(), T());
}



int main()
{
    constexpr int size = 8000;
    int* array = new int[size];

    srand(0);

    for(int i = 0; i < size; i++) {
        // array[i] = rand() % 10;
        array[i] = 1;
    }

    int sum = parallel_accumulate<std::remove_reference_t<decltype(*array)>>(array, array + size);

    std::cout << sum << std::endl;

    return 0;
}
