#include <iostream>
#include <cstdlib>
#include <ctime>

void GenerateLargeTest(int vertices, int edges) {
    std::srand(std::time(nullptr));
    std::cout << vertices << " " << edges << "\n";
    for (int i = 0; i < edges; ++i) {
        int u = std::rand() % vertices + 1;
        int v = std::rand() % vertices + 1;
        int weight = std::rand() % 100 + 1;
        if (u != v) {
            std::cout << u << " " << v << " " << weight << "\n";
        } else {
            --i; // Исключаем петли
        }
    }
}

int main() {
    GenerateLargeTest(100, 1000);
    return 0;
}
