#include <iostream>
#include <vector>
#include <cstdint>
#include <limits>
#include <algorithm>

using Point = std::pair<uint16_t, uint16_t>;

// find min in vector, returns index of min element
uint16_t Min(const std::vector<int64_t>& vec) {
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
uint16_t FindMinFareIndex(uint16_t n, uint16_t m, const std::vector<std::vector<int64_t>>& dp, uint16_t i, uint16_t j) {
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

std::vector<Point> FindPath(uint16_t n, uint16_t m, const std::vector<std::vector<int64_t>>& dp) {
    std::vector<Point> path;
    path.push_back(std::make_pair(n - 1, Min(dp[n - 1])));
    for (int16_t i = n - 2; i >= 0; --i) {
        path.push_back(std::make_pair(i, FindMinFareIndex(n, m, dp, path[path.size() - 1].first, path[path.size() - 1].second)));
    }

    return path;
}

std::pair<int64_t, std::vector<Point>> Solve(uint16_t n, uint16_t m, const std::vector<std::vector<int64_t>>& matrix) {
    std::vector<std::vector<int64_t>> dp(n, std::vector<int64_t>(m));
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
    uint16_t n, m;
    std::cin >> n >> m;
    std::vector<std::vector<int64_t>> matrix(n, std::vector<int64_t>(m));

    for (uint16_t i = 0; i < n; ++i) {
        for (uint16_t j = 0; j < m; ++j) {
            std::cin >> matrix[i][j];
        }
    }

    auto solution = Solve(n, m, matrix);

    std::cout << solution.first << std::endl;
    for (auto [i, j] : solution.second) {
        std::cout << '(' << i + 1 << ',' << j + 1 << ')' << ' ';
    }
    std::cout << std::endl;

    return 0;
}
