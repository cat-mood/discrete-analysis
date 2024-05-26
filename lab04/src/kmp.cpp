#include "kmp.h"
#include <iostream>

std::vector<int> ZFunction(const TString& s) {
    std::vector<int> z(s.size(), 0);
    int l = 0;
    int r = 0;
    for (int k = 1; k < s.size(); ++k) {
        if (k > r) {
            for (int j = 0; j < s.size(); ++j) {
                if (s[j] == s[j + k] && (j + k) < s.size()) {
                    ++z[k];
                } else {
                    break;
                }
            }
            if (z[k] > 0) {
                l = k;
                r = k + z[k] - 1;
            }
        } else {
            int kPrefix = k - l;
            if (z[kPrefix] < r - k + 1) {
                z[k] = z[kPrefix];
            } else {
                int q = r - k + 1;
                for (int j = r - l + 1; j < s.size(); ++j) {
                    if (s[j] == s[j + l] && (j + l) < s.size()) {
                        ++q;
                    } else {
                        break;
                    }
                }
                z[k] = q;
                r = q - 1;
                l = k;
            }
        }
        // std::cout << "k = " << k << " l = " << l << " r = " << r << " z = " << z[k] << std::endl;
    }

    return z;
}
