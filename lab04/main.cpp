#include <iostream>
#include "parser.h"
#include "kmp.h"

std::vector<TWord> copy(std::vector<TWord>& dest, int index) {
    std::vector<TWord> src;
    for (int i = index; i < dest.size(); i++) {
        src.emplace_back(dest[i]);
    }

    return src;
}

int main() {
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
                if (text.size() >= pattern.size() * 2) {
                    std::vector<int> result = KMPFunction(pattern, text);
                    for (int& id : result) {
                        printf("%d, %d\n", text[id].lineId + 1, text[id].wordId + 1);
                    }
                    text = copy(text, text.size() - pattern.size() + 1);
                }
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

    std::vector<int> result = KMPFunction(pattern, text);
    for (int& id : result) {
        printf("%d, %d\n", text[id].lineId + 1, text[id].wordId + 1);
    }

    return 0;
}
