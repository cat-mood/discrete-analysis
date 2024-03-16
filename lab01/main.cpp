#include <iostream>
#include <string>
#include "vector.h"
#include "pair.h"
#include "sort.h"

int main() {
    TVector<TPair> pairs;
    TPair pair;
    while (std::cin >> pair) {
        pairs.PushBack(std::move(pair));
    }
    RadixSort(pairs);
    for (int i = 0; i < pairs.Size(); ++i) {
        std::cout << pairs[i] << '\n';
    }

    return 0;
}
