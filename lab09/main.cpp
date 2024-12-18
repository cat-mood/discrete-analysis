#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cstdint>
#include <algorithm>
#include <climits>

using TVertex = uint16_t;
using TWeight = uint64_t;
using TGraph = std::vector<std::unordered_map<TVertex, TWeight>>;

// BFS
std::vector<TVertex> FindPath(TGraph graph, TVertex from, TVertex to) {
    std::queue<TVertex> toVisit;
    std::unordered_map<TVertex, TVertex> parent;

    toVisit.push(from);

    while (!toVisit.empty()) {
        TVertex current = toVisit.front();
        toVisit.pop();
        
        if (current == to) {
            std::vector<TVertex> path;
            path.push_back(current);

            while (current != from) {
                current = parent[current];
                path.push_back(current);
            }

            std::reverse(path.begin(), path.end());

            return path;
        }

        for (auto edge : graph[current]) {
            if (parent.find(edge.first) == parent.end()) {
                toVisit.push(edge.first);
                parent[edge.first] = current;
            }
        }
    }

    return std::vector<TVertex>();
}

int main() {
    uint16_t n, m;
    std::cin >> n >> m;
    
    TGraph graph = TGraph(n);

    for (uint16_t i = 0; i < m; ++i) {
        uint16_t from, to;
        uint64_t weight;
        std::cin >> from >> to >> weight;

        graph[from - 1][to - 1] = weight;
    }

    std::vector<TVertex> path = FindPath(graph, 0, n - 1);

    uint64_t maxCapacity = 0;

    while (!path.empty()) {
        uint64_t minCapacity = UINT64_MAX;
        for (uint16_t i = 0; i < path.size() - 1; ++i) {
            minCapacity = (graph[path[i]][path[i + 1]] < minCapacity) ? graph[path[i]][path[i + 1]] : minCapacity;
        }

        for (uint16_t i = 0; i < path.size() - 1; ++i) {
            graph[path[i]][path[i + 1]] -= minCapacity;
            if (graph[path[i]][path[i + 1]] == 0) {
                graph[path[i]].erase(path[i + 1]);
            }
            graph[path[i + 1]][path[i]] += minCapacity;
        }

        maxCapacity += minCapacity;

        path = FindPath(graph, 0, n - 1);
    }

    std::cout << maxCapacity << std::endl;

    return 0;
}
