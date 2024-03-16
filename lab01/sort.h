#pragma once

#include <vector>
#include "pair.h"
#include "vector.h"

TVector<TPair> CountingSort(TVector<TPair>& array, int pos);
void RadixSort(TVector<TPair>& array);
std::vector<TPair> CountingSort(std::vector<TPair>& array, int pos);
void RadixSort(std::vector<TPair>& array);
