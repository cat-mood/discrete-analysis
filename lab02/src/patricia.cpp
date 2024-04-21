#include "patricia.h"

TPatriciaTrie::TNode::TNode(const TData& data) : data{data} {
    children[0] = nullptr;
    children[1] = nullptr;
}

TPatriciaTrie::TPatriciaTrie() : root{nullptr} {}

void TPatriciaTrie::DestroyTrie(TNode* node) {
    if (node == nullptr) {
        return;
    }
    if (node->children[0]->bitNumber > node->bitNumber) {
        DestroyTrie(node->children[0]);
    }
    if (node->children[1] != nullptr && node->children[1]->bitNumber > node->bitNumber) {
        DestroyTrie(node->children[1]);
    }
    delete node;
}

TPatriciaTrie::~TPatriciaTrie() {
    DestroyTrie(root);
}

// if bitNumber == -1, it just finds node by key as usual; otherwise, it finds a place for new node with bitNumber
TPair<TPatriciaTrie::TNode*, int> TPatriciaTrie::FindPreviousNode(const std::string& key, int bitNumber) {
    if (root == nullptr) {
        return {root, -1};
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
    return {previous, currentDifferentBit};
}

// if bitNumber == -1, it just finds node by key as usual; otherwise, it finds a place for new node with bitNumber
TPatriciaTrie::TNode*& TPatriciaTrie::FindNode(const std::string& key, int bitNumber) {
    TPair<TNode*, int> previous = FindPreviousNode(key, bitNumber);
    if (previous.key == nullptr) {
        return root;
    }
    return previous.key->children[previous.value];
}

void TPatriciaTrie::Insert(const TData& data) {
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

const TPatriciaTrie::TData& TPatriciaTrie::Find(const std::string& key) {
    // std::cout << "finding " << key << std::endl;
    TNode* found = FindNode(key, -1);
    // std::cout << "found = " << found->data.key << std::endl;
    if (found == nullptr || BitDifference(key, found->data.key) != -1) {
        throw std::logic_error("NoSuchWord");
    }
    return found->data;
}

void TPatriciaTrie::Erase(const std::string& key) {
    // std::cout << "deleting " << key << std::endl;
    TNode*& deleting = FindNode(key, -1);
    if (deleting == nullptr) {
        throw std::logic_error("NoSuchWord");
    }
    // std::cout << "deleting = " << deleting->data.key << std::endl;
    if (BitDifference(key, deleting->data.key) != -1) {
        throw std::logic_error("NoSuchWord");
    }
    if (deleting->children[0] == deleting || deleting->children[1] == deleting) {
        // std::cout << "has selfpointer" << std::endl;
        if (deleting == root) {
            delete deleting;
            root = nullptr;
            return;
        }
        TNode*& deletingParentPointer = FindNode(key, deleting->bitNumber);
        if (deleting == deleting->children[0]) {
            deletingParentPointer = deleting->children[1];   // non-self pointer
            delete deleting;
            return;
        }
        deletingParentPointer = deleting->children[0];      // non-self pointer
        delete deleting;
        return;
    }
    // std::cout << "does not have selfpointer" << std::endl;
    // the node q has a backward pointer to node deleted
    TPair<TNode*, int> q = FindPreviousNode(key, -1);
    TNode*& backwardPointerToQ = FindNode(q.key->data.key, -1);
    TNode*& parentPointerToQ = FindNode(q.key->data.key, q.key->bitNumber);
    deleting->data = std::move(q.key->data);
    backwardPointerToQ = deleting;
    parentPointerToQ = q.key->children[!q.value];
    delete q.key;
}
