#include <iostream>
#include <vector>
#include <climits>
#include <chrono>

using duration_t = std::chrono::microseconds;
const std::string DURATION_PREFIX = "ms";

using namespace std;

void dfs(int i, int j, int current_penalty, vector<vector<int>>& A, int& min_penalty, vector<pair<int, int>>& path, vector<pair<int, int>>& best_path) {
    int n = A.size();
    int m = A[0].size();

    // Если достигли последней строки
    if (i == n - 1) {
        if (current_penalty < min_penalty) {
            min_penalty = current_penalty;
            best_path = path; // Сохраняем текущий путь
        }
        return;
    }

    // Переход в соседние клетки следующей строки
    for (int dj = -1; dj <= 1; ++dj) { // Соседние клетки в следующей строке
        int nj = j + dj;
        if (nj >= 0 && nj < m) { // Проверка границ
            path.push_back({i + 1, nj}); // Добавляем координаты в путь
            dfs(i + 1, nj, current_penalty + A[i + 1][nj], A, min_penalty, path, best_path);
            path.pop_back(); // Убираем последнюю координату после возврата
        }
    }
}

#include <iostream>
#include <vector>
#include <cstdint>
#include <limits>
#include <algorithm>

using Point = std::pair<uint16_t, uint16_t>;

// find min in vector, returns index of min element
uint16_t Min(const std::vector<int>& vec) {
    int64_t min = INT64_MAX;
    uint16_t minIndex = 0;
    for (uint16_t i = 0; i < vec.size(); ++i) {
        if (vec[i] < min) {
            min = vec[i];
            minIndex = i;
        }
    }

    return minIndex;
}

// find min fare index among top neighbors
uint16_t FindMinFareIndex(uint16_t n, uint16_t m, const std::vector<std::vector<int>>& dp, uint16_t i, uint16_t j) {
    std::vector<uint16_t> neighbors;
    neighbors.push_back(j);
    if (j - 1 >= 0) {
        neighbors.push_back(j - 1);
    }
    if (j + 1 < m) {
        neighbors.push_back(j + 1);
    }
    int64_t minFare = INT64_MAX;
    uint16_t minFareIndex = j;
    for (uint16_t idx : neighbors) {
        if (dp[i - 1][idx] < minFare) {
            minFare = dp[i - 1][idx];
            minFareIndex = idx;
        }
    }

    return minFareIndex;
}

std::vector<Point> FindPath(uint16_t n, uint16_t m, const std::vector<std::vector<int>>& dp) {
    std::vector<Point> path;
    path.push_back(std::make_pair(n - 1, Min(dp[n - 1])));
    for (int16_t i = n - 2; i >= 0; --i) {
        path.push_back(std::make_pair(i, FindMinFareIndex(n, m, dp, path[path.size() - 1].first, path[path.size() - 1].second)));
    }

    return path;
}

std::pair<int64_t, std::vector<Point>> Solve(uint16_t n, uint16_t m, const std::vector<std::vector<int>>& matrix) {
    std::vector<std::vector<int>> dp(n, std::vector<int>(m));
    dp[0] = matrix[0];
    for (uint16_t i = 1; i < n; ++i) {
        for (uint16_t j = 0; j < m; ++j) {
            uint16_t minFareIndex = FindMinFareIndex(n, m, dp, i, j);
            dp[i][j] = dp[i - 1][minFareIndex] + matrix[i][j];
        }
    }

    uint16_t minFareIndex = Min(dp[n - 1]);
    std::vector<Point> path(FindPath(n, m, dp));
    std::reverse(path.begin(), path.end());

    return std::make_pair(dp[n - 1][minFareIndex], path);
}

int main() {
    std::chrono::time_point<std::chrono::system_clock> start_ts = std::chrono::system_clock::now();
    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<int>> A(n, std::vector<int>(m));
    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < m; ++j)
        {
            std::cin >> A[i][j];
        }
    }
    int min_penalty = INT_MAX;
    vector<pair<int, int>> best_path;

    // Запускаем DFS для каждой клетки первой строки
    for (int j = 0; j < m; ++j) {
        vector<pair<int, int>> path = {{0, j}}; // Начинаем с текущей клетки
        dfs(0, j, A[0][j], A, min_penalty, path, best_path);
    }

    // // Вывод результата
    // cout << min_penalty << endl;
    // for (const auto& p : best_path) {
    //     cout << "(" << p.first + 1 << "," << p.second + 1 << ") ";
    // }

    std::cout << "\n";
    auto end_ts = std::chrono::system_clock::now();
    uint64_t sort_ts = std::chrono::duration_cast<duration_t>( end_ts - start_ts ).count();
    std::cout << "naive's time: " << sort_ts << DURATION_PREFIX << "\n";

    start_ts = std::chrono::system_clock::now();
    Solve(n, m, A);
    end_ts = std::chrono::system_clock::now();
    sort_ts = std::chrono::duration_cast<duration_t>( end_ts - start_ts ).count();
    std::cout << "my solution's time: " << sort_ts << DURATION_PREFIX << "\n";
}
