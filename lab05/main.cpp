#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>


const char SENTINEL = '$';

class TSuffixTree {
    private:
        class TNode {
            public:
                int begin;
                int* end;
                TNode* suffixLink;
                bool isLeaf;
                std::unordered_map<char, TNode *> children;

                TNode(int start, int* finish, TNode* suffixLink, bool leaf);
                ~TNode() = default;
        };
    
        struct TreeData {
            TNode* currentNode;
            int currentIndex;
            int jumpCounter;
            int plannedSuffixes;
            TNode* lastInnerNode;
        };


        TNode* root;
        std::string str;
        int suffixTreeEnd;
        TreeData params;

        void CreateTree();
        void AddSuffix(int position);
        void DestroyTree(TNode* node);

        static int CurveLength(TNode* node);
        void SplitCurve(TNode* node, int position);

    public:
        TSuffixTree(std::string& input_str);
        ~TSuffixTree();
        void MatchingStatistic(std::vector<int> &ms, const std::string &str);
};


TSuffixTree::TNode::TNode(int start, int *finish, TNode *suffixLink, bool leaf) {
    begin = start;
    end = finish;
    this->suffixLink = suffixLink;
    isLeaf = leaf;
}


TSuffixTree::TSuffixTree(std::string& input_str) {
    str = std::move(input_str);
    root = new TNode(-1, new int(-1), nullptr, false);
    suffixTreeEnd = -1;

    str += SENTINEL;
    CreateTree();
}


void TSuffixTree::DestroyTree(TNode* node) {
    if (!node) {
        return;
    }

    if (!(node->isLeaf)) {
        delete node->end;
    }

    for (auto iter : node->children) {
        DestroyTree(iter.second);
    }

    delete node;
}


TSuffixTree::~TSuffixTree() {
    DestroyTree(root);
}


void TSuffixTree::CreateTree() {
    params.currentIndex = -1;
    params.jumpCounter = 0;
    params.plannedSuffixes = 0;
    params.currentNode = root;
    params.lastInnerNode = nullptr;

    for (int i = 0; i < str.length(); i++) {
        AddSuffix(i);
    }
}


int TSuffixTree::CurveLength(TNode* node) {
    return *(node->end) - node->begin + 1;
}


void TSuffixTree::SplitCurve(TNode* node, int position) {
    TNode* innerNode = new TNode(node->begin, new int(node->begin + params.jumpCounter - 1), root, false);
    params.currentNode->children[str[params.currentIndex]] = innerNode;

    node->begin += params.jumpCounter;
    
    innerNode->children[str[position]] = new TNode(position, &suffixTreeEnd, nullptr, true);
    innerNode->children[str[node->begin]] = node;

    if (params.lastInnerNode) {
        params.lastInnerNode->suffixLink = innerNode;
    }
    params.lastInnerNode = innerNode;
}


void TSuffixTree::AddSuffix(int position) {
    params.lastInnerNode = nullptr;
    params.plannedSuffixes++;
    suffixTreeEnd++;

    while (params.plannedSuffixes) {
        if (!(params.jumpCounter)) {
           params.currentIndex = position;
        }
        
        auto searchIter = params.currentNode->children.find(str[params.currentIndex]);
        TNode* nextNode = (searchIter != params.currentNode->children.end()) ? searchIter->second : nullptr;

        if (!nextNode) {
            params.currentNode->children[str[params.currentIndex]] = new TNode(position, &suffixTreeEnd, nullptr, true);
        } else {
            if (str[position] == str[nextNode->begin + params.jumpCounter]) {
                params.jumpCounter++;
                break;
            }

            int curveLen = CurveLength(nextNode);
            
            if (params.jumpCounter >= curveLen) {
                params.currentIndex += curveLen;
                params.jumpCounter -= curveLen;
                params.currentNode = nextNode;
                continue;
            }
            
            SplitCurve(nextNode, position);
        }

        if (params.currentNode == root) {
            params.currentIndex++;
            if (params.jumpCounter) {
                params.jumpCounter--;
            }
            params.plannedSuffixes--;
        } else {
            params.currentNode = params.currentNode->suffixLink;
        }
    }
}


void TSuffixTree::MatchingStatistic(std::vector<int> &ms, const std::string &text) {
    size_t len_t = text.length();
    ms.assign(len_t, 0);
    
    for (size_t i = 0; i < len_t; ++i) {
        TNode* currentNode = root;
        size_t j = i;
        size_t skipEqual = 0;

        while (j < len_t) {

            if (!currentNode) {
                currentNode = root;
            }

            if (currentNode->children.find(text[j]) != currentNode->children.end()) {
                TNode *next_node = currentNode->children[text[j]];
                size_t curve_len = CurveLength(next_node);
                
                while (skipEqual < curve_len && j < len_t && text[j] == str[next_node->begin + skipEqual]) {
                    ms[i]++;
                    j++;
                    skipEqual++;
                }
                
                if (skipEqual < curve_len) {
                    currentNode = currentNode->suffixLink;

                    break;
                } else {
                    currentNode = next_node;
                    skipEqual = 0;
                }
            } else {
                break;
            }
        }
    }
}


int main() {

    std::string str;
    std::cin >> str;
    int patternLen = str.length();

    TSuffixTree t(str);
    std::vector<int> ms;

    std::string text;
    std::cin >> text;

    t.MatchingStatistic(ms, text);

    for (size_t i = 0; i < ms.size(); i++) {
        if (ms[i] == patternLen) {
            std::cout << i + 1 << std::endl;
        }
    }

    return 0;
}
