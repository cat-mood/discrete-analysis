#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

void GenerateLargeSubstringTest(size_t textLength, size_t substringLength) {
    std::srand(std::time(nullptr));
    
    // Генерация текста
    std::string text;
    for (size_t i = 0; i < textLength; ++i) {
        text += 'a' + std::rand() % 26; // Случайная буква от 'a' до 'z'
    }

    // Генерация подстроки
    std::string substring;
    for (size_t i = 0; i < substringLength; ++i) {
        substring += 'a' + std::rand() % 26;
    }

    // Вывод
    std::cout << substring << "\n";
    std::cout << text << "\n";
}

int main() {
    GenerateLargeSubstringTest(100000, 100); // Генерация текста длиной 100,000 и подстроки длиной 100
    return 0;
}
