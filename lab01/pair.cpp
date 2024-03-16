#include "pair.h"

std::istream& operator>>(std::istream& is, TPair& rhs) {
    rhs.key = "";
    char token;
    for (int i = 0; i < KEY_LENGTH; ++i) {
        is >> token;
        rhs.key += token;
    }
    is >> rhs.value;
    return is;
}

std::ostream& operator<<(std::ostream& os, const TPair& rhs) {
    int pos = 0;
    for (char token : rhs.key) {
        os << token;
        if (pos == 0 || pos == 3) {
            os << ' ';
        }
        ++pos;
    }
    os << '\t' << rhs.value;
    return os;
}

TPair::TPair(const TPair& other) : key{other.key}, value{other.value} {}

TPair::TPair(TPair&& other) {
    key = std::move(other.key);
    value = std::move(other.value);
}

TPair& TPair::operator=(const TPair& other) {
    key = other.key;
    value = other.value;
    return *this;
}

TPair& TPair::operator=(TPair&& other) noexcept {
    key = std::move(other.key);
    value = std::move(other.value);
    return *this;
}

bool TPair::operator<(const TPair& other) const {
    return key < other.key;
}
