#include <iostream>
#include <cstdint>
#include <algorithm>
#include <vector>

// #define DEBUG 1

struct Segment {
    int64_t start;
    int64_t end;
    size_t index;

    Segment() = default;

    Segment(int64_t start, int64_t end, size_t index) : start{start}, end{end}, index{index} {}

    int64_t ComputeCover(int64_t left, int64_t m) {
        if (end < 0 || start > m) {
            return 0;
        }
        return std::min<int64_t>(m, end) - std::max<int64_t>(left, start);
    }

    // bool operator<(const Segment& rhs) const {
    //     if (start == rhs.start) {
    //         return end < rhs.end;
    //     }
    //     return start < rhs.start;
    // }
};

bool IndexComparator(const Segment& a, const Segment& b) {
    return a.index < b.index;
}

bool SegmentComparator(const Segment& a, const Segment& b) {
    if (a.start == b.start) {
            return a.end < b.end;
        }
    return a.start < b.start;
}

int main() {
    size_t n;
    std::cin >> n;
    std::vector<Segment> segments(n);
    for (size_t i = 0; i < n; ++i) {
        int64_t start, end;
        std::cin >> start >> end;
        segments[i] = Segment(start, end, i);
    }
    int64_t m;
    std::cin >> m;
    std::sort(segments.begin(), segments.end(), SegmentComparator);
    
    int64_t left = 0;
    size_t i = 0;
    std::vector<Segment> answer;
    while (left < m && i < n) {
        uint64_t maxCoverIndex = 0;
        while ((segments[i].end < left || segments[i].start > left) && i < n) {
            ++i;
        }
        while (segments[i].start <= left && i < n) {
            #ifdef DEBUG
                int cover1 = segments[maxCoverIndex].ComputeCover(left, m);
                int cover2 = segments[i].ComputeCover(left, m);
            #endif
            if (segments[maxCoverIndex].ComputeCover(left, m) <= segments[i].ComputeCover(left, m)) {
                maxCoverIndex = i;
            }
            ++i;
        }
        answer.push_back(segments[maxCoverIndex]);
        left = segments[maxCoverIndex].end;
    }

    if (left < m) {
        std::cout << 0 << std::endl;
    } else {
        std::cout << answer.size() << std::endl;
        std::sort(answer.begin(), answer.end(), IndexComparator);
        for (Segment seg : answer) {
            std::cout << seg.start << ' ' << seg.end << std::endl;
        }
    }

    return 0;
}
