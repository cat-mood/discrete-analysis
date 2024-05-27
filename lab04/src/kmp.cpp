#include "kmp.h"

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
    std::vector<int> z;
    z = ZFunction(s);
    for (int j = s.size() - 1; j >= 1; --j) {
        int i = j + z[j] - 1;
        sp[i] = z[j];
    }

    return sp;
}

std::vector<int> KMPFunction(const TString& pattern, const TString& text) {
    std::vector<int> result;
    std::vector<int> sp = SPFunction(pattern);
    int m = text.size();      // text line length
    int n = pattern.size();     // pattern length
    int i = 0;
    if (n > m) {
		return result;
	}
	while (i < m - n + 1) {
		int j = 0;
		while (j < n and pattern[j] == text[i + j]) {
			++j;
		}
		if (j == n) {
			result.push_back(i);
		} else {
			if (j > 0 and j > sp[j - 1]) {
				i = i + j - sp[j - 1] - 1;
			}
		}
		++i;
	}

    return result;
}
