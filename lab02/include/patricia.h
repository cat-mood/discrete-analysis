#pragma once

#include "binary_string.h"
#include <stdexcept>

struct TPair {
    std::string key;
    uint64_t value;

    TPair(const std::string& key, uint64_t value);
};

class TPatriciaTrie {
private:
    struct TNode {
        TPair data;
        TNode* children[2];
        int bitNumber;

        TNode(const TPair& data);
    };

    TNode* root;

    TNode*& FindNode(const std::string& key, int bitNumber = -1);
public:
    TPatriciaTrie();
    ~TPatriciaTrie() = default;     // temporary (don't forget to change!)
    void Insert(const TPair& data);
    const TPair& Find(const std::string& key);
    void Erase(const std::string& key);
};
