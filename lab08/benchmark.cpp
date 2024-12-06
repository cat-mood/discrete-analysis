#include <iostream>
#include <vector>
#include <chrono>
using namespace std;

using duration_t = std::chrono::microseconds;

// Структура для хранения отрезков
struct NaiveSegment {
    int L, R;
    int index; // Индекс для вывода в порядке ввода
};

// Функция для нахождения минимального покрытия интервала [0, M]
void findMinimumCover(vector<NaiveSegment>& segments, int M) {
    int current_end = 0;   // Текущий конец покрытия
    int best_end = 0;      // Лучшая правая граница для текущего шага
    int best_index = -1;   // Индекс лучшего сегмента
    vector<NaiveSegment> result;

    while (current_end < M) {
        best_end = current_end;

        // Проходим по всем сегментам
        for (int i = 0; i < segments.size(); ++i) {
            // Если сегмент начинается не позже текущего конца покрытия и имеет наибольшую правую границу
            if (segments[i].L <= current_end && segments[i].R > best_end) {
                best_end = segments[i].R;
                best_index = i;
            }
        }

        // Если ни один сегмент не улучшает покрытие
        if (best_end == current_end) {
            cout << "0" << endl;
            return;
        }

        // Добавляем лучший сегмент к результату
        result.push_back(segments[best_index]);
        current_end = best_end; // Обновляем текущий конец покрытия
    }

    // // Вывод результата
    // cout << result.size() << endl;
    // for (const auto& seg : result) {
    //     cout << seg.L << " " << seg.R << endl;
    // }
}

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

std::vector<Segment> Solve(size_t n, std::vector<Segment> segments, int64_t m) {
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

    return answer;
}


int main() {
    int N;
    cin >> N;

    vector<NaiveSegment> naiveSegments(N);
    vector<Segment> segments(N);
    for (int i = 0; i < N; ++i) {
        int l, r;
        cin >> l >> r;
        naiveSegments[i].L = l;
        naiveSegments[i].R = r;
        naiveSegments[i].index = i; // Сохраняем порядок ввода
        segments[i].start = l;
        segments[i].end = r;
        segments[i].index = i;
    }

    int M;
    cin >> M;


    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    findMinimumCover(naiveSegments, M);
    end = std::chrono::system_clock::now();
    std::cout << "Naive: " << std::chrono::duration_cast<duration_t>( end - start ).count() << " ms" << std::endl;

    start = std::chrono::system_clock::now();
    Solve(N, segments, M);
    end = std::chrono::system_clock::now();
    std::cout << "My solution: " << std::chrono::duration_cast<duration_t>( end - start ).count() << " ms" << std::endl;


    return 0;
}
