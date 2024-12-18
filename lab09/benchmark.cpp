#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cstdint>
#include <algorithm>
#include <climits>
#include <stack>
#include <unordered_set>
#include <chrono>

using TVertex = uint16_t;
using TWeight = uint64_t;
using TGraph = std::vector<std::unordered_map<TVertex, TWeight>>;

using duration_t = std::chrono::microseconds;

// DFS
std::vector<TVertex> FindPathDFS(TGraph graph, TVertex from, TVertex to) {
    std::stack<TVertex> toVisit; // Стек для DFS
    std::unordered_map<TVertex, TVertex> parent; // Хранение пути
    std::unordered_set<TVertex> visited; // Множество посещённых вершин

    toVisit.push(from);

    while (!toVisit.empty()) {
        TVertex current = toVisit.top();
        toVisit.pop();

        if (visited.find(current) != visited.end()) {
            continue;
        }

        visited.insert(current);

        if (current == to) {
            // Восстановление пути
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
            TVertex neighbor = edge.first;
            if (visited.find(neighbor) == visited.end() && parent.find(neighbor) == parent.end()) {
                parent[neighbor] = current;
                toVisit.push(neighbor);
            }
        }
    }

    return std::vector<TVertex>(); // Если путь не найден
}

std::vector<TVertex> FindPathBFS(TGraph graph, TVertex from, TVertex to) {
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

uint64_t BFSSolve(uint16_t n, uint16_t m, TGraph graph) {
    std::vector<TVertex> path = FindPathBFS(graph, 0, n - 1);

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

        path = FindPathBFS(graph, 0, n - 1);
    }

    return maxCapacity;
}

uint64_t DFSSolve(uint16_t n, uint16_t m, TGraph graph) {
    std::vector<TVertex> path = FindPathDFS(graph, 0, n - 1);

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

        path = FindPathDFS(graph, 0, n - 1);
    }

    return maxCapacity;
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

    std::chrono::time_point<std::chrono::system_clock> start, end;

    start = std::chrono::system_clock::now();
    BFSSolve(n, m, graph);
    end = std::chrono::system_clock::now();
    std::cout << "BFS: " << std::chrono::duration_cast<duration_t>( end - start ).count() << " ms" << std::endl;

    start = std::chrono::system_clock::now();
    DFSSolve(n, m, graph);
    end = std::chrono::system_clock::now();
    std::cout << "DFS: " << std::chrono::duration_cast<duration_t>( end - start ).count() << " ms" << std::endl;

    return 0;
}
