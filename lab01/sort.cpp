#include <climits>
#include "sort.h"

TVector<TPair> CountingSort(TVector<TPair>& array, int pos) {
    TVector<int> count(CHAR_MAX + 1, 0);
    for (int i = 0; i < array.Size(); ++i) {
        ++count[array[i].key[pos]];
    }
    for (int i = 1; i < count.Size(); ++i) {
        count[i] += count[i - 1];
    }
    TVector<TPair> result(array.Size());
    for (int i = array.Size() - 1; i >= 0; --i) {
        result[--count[array[i].key[pos]]] = std::move(array[i]);
    }
    return result;
}

void RadixSort(TVector<TPair>& array) {
    for (int i = KEY_LENGTH - 1; i >= 0; --i) {
        array = CountingSort(array, i);
    }
}

std::vector<TPair> CountingSort(std::vector<TPair>& array, int pos) {
    std::vector<int> count(CHAR_MAX + 1, 0);
    for (int i = 0; i < array.size(); ++i) {
        ++count[array[i].key[pos]];
    }
    for (int i = 1; i < count.size(); ++i) {
        count[i] += count[i - 1];
    }
    std::vector<TPair> result(array.size());
    for (int i = array.size() - 1; i >= 0; --i) {
        result[--count[array[i].key[pos]]] = std::move(array[i]);
    }
    return result;
}

void RadixSort(std::vector<TPair>& array) {
    for (int i = KEY_LENGTH - 1; i >= 0; --i) {
        array = CountingSort(array, i);
    }
}
