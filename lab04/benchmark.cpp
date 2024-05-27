#include <iostream>
#include <string>
#include "kmp.h"
#include "parser.h"
#include <chrono>

using duration_t = std::chrono::microseconds;

std::vector<TWord> copy(std::vector<TWord>& dest, int index) {
    std::vector<TWord> src;
    for (int i = index; i < dest.size(); i++) {
        src.emplace_back(dest[i]);
    }

    return src;
}

std::vector<int> NaiveFunction(std::string& pattern, std::string& text) {
    std::vector<int> result;
    for (int i = 0; i < text.size(); ++i) {
        int j = 0;
        while (j < pattern.size() && pattern[j] == text[i + j]) {
            ++j;
        }
        if (j == pattern.size()) {
            result.push_back(i);
        }
    }

    return result;
}

int main() {
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::string s_pattern;
    getline(std::cin, s_pattern);
    std::string s_text;
    getline(std::cin, s_text);
    start = std::chrono::system_clock::now();
    auto s_result = NaiveFunction(s_pattern, s_text);
    end = std::chrono::system_clock::now();
    std::cout << "Naive: " << std::chrono::duration_cast<duration_t>( end - start ).count() << " ms" << std::endl;
    TString pattern;
    TString text;
    TWord word;
    bool isNewWord = false;
    char cur = getchar();
    int i = 0;
    while (true) {
        if (cur == ' ' || cur == '\t' || cur == '\n') {
            if (isNewWord) {
                pattern.emplace_back(word);
                isNewWord = false;
            }
            // printf("%s\n", word.word);
            word.Clear();
            i = 0;
            if (cur == '\n') {
                break;
            }
        } else {
            isNewWord = true;
            cur = std::tolower(cur);
            word[i] = cur;
            ++i;
        }
        cur = getchar();
    }
    int line = 0;
    int column = 0;
    cur = getchar();
    i = 0;
    while (cur != EOF) {
        if (cur == ' ' || cur == '\t' || cur == '\n') {
            if (isNewWord) {
                word.lineId = line;
                word.wordId = column;
                text.emplace_back(word);
                ++column;
                isNewWord = false;
            }
            word.Clear();
            i = 0;
            if (cur == '\n') {
                ++line;
                column = 0;
            }
        } else {
            isNewWord = true;
            cur = std::tolower(cur);
            word[i] = cur;
            ++i;
        }
        cur = getchar();
    }
    if (isNewWord) {
        word.lineId = line;
        word.wordId = column;
        text.emplace_back(word);
    }
    start = std::chrono::system_clock::now();
    std::vector<int> result = KMPFunction(pattern, text);
    end = std::chrono::system_clock::now();
    std::cout << "KMP: " << std::chrono::duration_cast<duration_t>( end - start ).count() << " ms" << std::endl;
}
