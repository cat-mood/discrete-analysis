#pragma once

#include <string>
#include <vector>

const int MAX_WORD_LEN = 16;

struct TWord {
    char word[MAX_WORD_LEN];
    int lineId, wordId;

    TWord();
    void Clear();
    char& operator[](int index);
    bool operator==(const TWord& rhs) const;
    bool operator!=(const TWord& rhs) const;
};

using TString = std::vector<TWord>;
