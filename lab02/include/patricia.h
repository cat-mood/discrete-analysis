#pragma once

#include "binary_string.h"
#include <stdexcept>

template <class T, class U>
struct TPair {
    T key;
    U value;

    TPair() = default;
    TPair(const T& key, const U& value) : key(key), value(value) {}
    TPair(T&& key, U&& value) : key(key), value(value) {}
    TPair(const TPair& other) : key(other.key), value(other.value) {}
    TPair(TPair&& other) noexcept : key(std::move(other.key)), value(std::move(other.value)) {}
    TPair& operator=(const TPair& other) {
        key = other.key;
        value = other.value;
        return *this;
    }
    TPair& operator=(TPair&& other) noexcept {
        key = std::move(other.key);
        value = std::move(other.value);
        return *this;
    }
};

class TPatriciaTrie {
private:
    using TData = TPair<std::string, uint64_t>;

    struct TNode {
        TData data;
        TNode* children[2];
        int bitNumber;

        TNode(const TData& data);
    };

    TNode* root;

    TNode*& FindNode(const std::string& key, int bitNumber);
    TPair<TPatriciaTrie::TNode*, int> FindPreviousNode(const std::string& key, int bitNumber);
public:
    TPatriciaTrie();
    ~TPatriciaTrie() = default;     // temporary (don't forget to change!)
    void Insert(TData&& data);
    const TData& Find(const std::string& key);
    void Erase(const std::string& key);
};
