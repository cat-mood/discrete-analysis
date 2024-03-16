#include <iostream>
#include <chrono>
#include <algorithm>
#include "vector.h"
#include "pair.h"
#include "sort.h"

using TDuration = std::chrono::microseconds;
const std::string DURATION_PREFIX = "us";

int main() {
    std::vector<TPair> pairs, stl_pairs;
    TPair pair;
    while (std::cin >> pair) {
        pairs.push_back(std::move(pair));
    }
    stl_pairs = pairs;

    std::cout << "Count of lines is " << pairs.size() << std::endl;

    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    RadixSort(pairs);
    auto end = std::chrono::system_clock::now();
    uint64_t countingSort = std::chrono::duration_cast<TDuration>(end - start).count();

    start = std::chrono::system_clock::now();
    std::stable_sort(std::begin(stl_pairs), std::end(stl_pairs));
    end = std::chrono::system_clock::now();

    uint64_t stlSort = std::chrono::duration_cast<TDuration>(end - start).count();
    std::cout << "Radix sort time: " << countingSort << DURATION_PREFIX << std::endl;
    std::cout << "STL stable sort time: " << stlSort << DURATION_PREFIX << std::endl;

    return 0;
}
