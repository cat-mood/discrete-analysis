#include "parser.h"

TWord::TWord() {
    for (int i = 0; i < MAX_WORD_LEN; ++i) {
        word[i] = 0;
    }
    lineId = 0;
    wordId = 0;
}

void TWord::Clear() {
    for (unsigned short i = 0; i < MAX_WORD_LEN; ++i) {
		word[i] = 0;
	}
}

char& TWord::operator[](int index) {
    return word[index];
}

bool TWord::operator==(const TWord& rhs) const {
    for (unsigned short i = 0; i < MAX_WORD_LEN; ++i) {
        if (word[i] != rhs.word[i]) {
            return false;
        }
    }

    return true;
}

bool TWord::operator!=(const TWord& rhs) const {
    for (unsigned short i = 0; i < MAX_WORD_LEN; ++i) {
        if (word[i] == rhs.word[i]) {
            return false;
        }
    }

    return true;
}
