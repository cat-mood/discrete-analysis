#pragma once

#include <iostream>
#include <string>

const int KEY_LENGTH = 6;

struct TPair {
    std::string key;
    std::string value;

    friend std::istream& operator>>(std::istream& is, TPair& rhs);
    friend std::ostream& operator<<(std::ostream& os, const TPair& rhs);
    TPair() = default;
    TPair(const TPair& other);
    TPair(TPair&& other);
    TPair& operator=(const TPair& other);
    TPair& operator=(TPair&& other) noexcept;
    bool operator<(const TPair& other) const;
};
