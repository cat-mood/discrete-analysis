#include "patricia.h"

#include <iostream> // testing

TPair::TPair(const std::string& key, uint64_t value) : key{key}, value{value} {}

TPatriciaTrie::TNode::TNode(const TPair& data) : data{data} {
    children[0] = nullptr;
    children[1] = nullptr;
}

TPatriciaTrie::TPatriciaTrie() : root{nullptr} {}

// if bitNumber == -1, it just finds node by key as usual; otherwise, it finds a place for new node with bitNumber
TPatriciaTrie::TNode*& TPatriciaTrie::FindNode(const std::string& key, int bitNumber) {
    if (root == nullptr) {
        return root;
    }
    // std::cout << "root = " << root->data.key << std::endl;
    // std::cout << "bitNumber = " << bitNumber << std::endl;
    TNode* previous = root;
    int currentDifferentBit = 0;
    TNode* current = previous->children[currentDifferentBit];
    while (current->bitNumber > previous->bitNumber && (current->bitNumber < bitNumber || bitNumber == -1)) {
        // std::cout << "current = " << current->data.key << std::endl;
        // std::cout << "current->bitNumber = " << current->bitNumber << std::endl;
        currentDifferentBit = GetBitByIndex(key, current->bitNumber);
        previous = current;
        current = previous->children[currentDifferentBit];
    }
    // std::cout << "previous = " << previous->data.key << std::endl;
    return previous->children[currentDifferentBit];
}

void TPatriciaTrie::Insert(const TPair& data) {
    // std::cout << "inserting " << data.key << std::endl;
    if (root == nullptr) {
        TNode* inserting = new TNode{data};
        root = inserting;
        root->children[0] = root;
        root->bitNumber = -1;
        return;
    }
    TNode* found = FindNode(data.key, -1);
    // std::cout << "found = " << found->data.key << std::endl;
    int bitIndex = BitDifference(data.key, found->data.key);
    if (bitIndex == -1) {
        throw std::logic_error("Exist");
    }
    TNode* inserting = new TNode{data};
    inserting->bitNumber = bitIndex;
    int differentBit = GetBitByIndex(data.key, bitIndex);
    TNode*& place = FindNode(data.key, bitIndex);
    // std::cout << "place = " << place->data.key << std::endl;
    inserting->children[differentBit] = inserting;
    inserting->children[!differentBit] = place;
    place = inserting;
    // std::cout << "root->children[0] = " << root->children[0]->data.key << std::endl;
}

const TPair& TPatriciaTrie::Find(const std::string& key) {
    // std::cout << "finding " << key << std::endl;
    TNode* found = FindNode(key, -1);
    // std::cout << "found = " << found->data.key << std::endl;
    if (found == nullptr || BitDifference(key, found->data.key) != -1) {
        throw std::logic_error("NoSuchWord");
    }
    return found->data;
}

void TPatriciaTrie::Erase(const std::string& key) {
    

}
