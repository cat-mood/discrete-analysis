#include "kmp.h"
#include <iostream>

std::vector<int> ZFunction(const TString& s) {
    std::vector<int> z(s.size(), 0);
    int l = 0;
    int r = 0;
    for (int i = 1; i < s.size(); ++i) {
        z[i] = std::max(0, std::min(r - i, z[i - l]));
        while (i + z[i] < s.size() && s[z[i]] == s[i + z[i]]) {
            ++z[i];
        }
        if (i + z[i] > r) {
            l = i;
            r = i + z[i];
        }
    }

    return z;
}

std::vector<int> SPFunction(const TString& s) {
    std::vector<int> sp(s.size(), 0);
    std::vector<int> z = ZFunction(s);
    for (int j = s.size() - 1; j >= 1; --j) {
        int i = j + z[j] - 1;
        sp[i] = z[j];
    }

    return sp;
}
