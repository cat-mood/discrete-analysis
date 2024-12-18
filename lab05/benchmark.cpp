#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <iterator>
#include <chrono>

const char TERMINAL = '$';

using vec_t = std::vector<size_t>;
using duration_t = std::chrono::microseconds;

class SuffixNode {
public:
    int number = -1;
    std::map<char, SuffixNode> m_data;
};

class SuffixTree {
public:
    SuffixTree( const std::string & pattern ) { Build( pattern + TERMINAL ); }
    ~SuffixTree() {}
    bool Search( const std::string & text, vec_t &pos ); 
private:
    void Build( const std::string & );
    void AddSuffix( const std::string &, size_t );
    void CollectPositions( const SuffixNode *node, vec_t &pos );
private:
    SuffixNode root;
};

void
SuffixTree::Build( const std::string & pattern ) {
    for( size_t i = 0; i < pattern.size(); ++i ) {
        std::string suffix = pattern.substr( i, pattern.size() - i );
        AddSuffix( suffix, i+1 );
    }
}

void
SuffixTree::AddSuffix( const std::string & suffix, size_t number) {
    SuffixNode *node = &root;
    for( size_t i = 0; i < suffix.size(); ++i ) {
        const char *sign = &suffix[i];
        auto it = node->m_data.find( *sign );
        // Если дуги с такой меткой нет, тогда создадим её. 
        if( it == node->m_data.end() ) {
            SuffixNode new_node = SuffixNode();
            // Лист? Тогда присвоим ему номер суффикса.
            if( i == suffix.size() - 1 ) {
                new_node.number = number;
            }
            node->m_data[*sign] = new_node;
        }
        node = &(node->m_data[*sign]);
    }
}

void
SuffixTree::CollectPositions( const SuffixNode *node, vec_t &pos ) {
    for( auto const &elem : node->m_data ) {
        if( elem.first == TERMINAL ) {
            pos.push_back( elem.second.number );
        }
        
        CollectPositions( &(elem.second), pos );
    }
}

bool
SuffixTree::Search( const std::string & text, vec_t &pos ) {
    SuffixNode *node = &root;
    for( size_t i = 0; i < text.size(); ++i ) {
        const char *sign = &text[i];
        auto it = node->m_data.find( *sign );
        // Если нет ребра с такой меткой, то текст мы не нашли.
        if( it == node->m_data.end() ) {
            return false;
        }
        node = &(node->m_data[*sign]);

        // Это последний символ? Тогда найдём все позиции в оставшемся
        // поддереве.
        if( i == text.size() - 1 ) {
            CollectPositions( node, pos );
            std::sort( pos.begin(), pos.end() );
        }
    }
    return true;
}

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


void MatchingSolve(const std::string& text, std::string& pattern) {
    int patternLen = pattern.length();

    TSuffixTree t(pattern);
    std::vector<int> ms;

    t.MatchingStatistic(ms, text);
    // for (size_t i = 0; i < ms.size(); i++) {
    //     if (ms[i] == patternLen) {
    //         std::cout << i + 1 << std::endl;
    //     }
    // }
}


void NaiveSolve(const std::string& text, const std::string& pattern) {
    SuffixTree root( pattern );
    vec_t pos;
    // Образец не встретился в суффиксе.
    root.Search( text, pos );
    // Пример вывода:
    // 1: 1
    // 2: 2
    // 3: 2, 6
    // std::stringstream str;
    // const char * delim = ", ";
    // // Небольшая хитрость: копируем в поток элементы векторая, чтобы элементы разделялись delim.
    // // Если сделать просто std::copy( pos.begin(), pos.end(), ... ), тогда после последнего 
    // // элемента будет символ delim. А вообще есть boost::algorithm::join(pos, delim), но его 
    // // использовать в лабораторных нельзя!
    // std::copy( pos.begin(), pos.end()-1, std::ostream_iterator<size_t>(str, delim) ); 
    // std::copy( pos.end()-1, pos.end(), std::ostream_iterator<size_t>(str));
    // // std::cout << i << ": " << str.str() << std::endl;
}

int main() {
    std::string text, pattern;
    std::cin >> pattern;

    std::chrono::time_point<std::chrono::system_clock> start, end;

    start = std::chrono::system_clock::now();
    NaiveSolve(text, pattern);
    end = std::chrono::system_clock::now();
    std::cout << "Naive: " << std::chrono::duration_cast<duration_t>( end - start ).count() << " ms" << std::endl;

    start = std::chrono::system_clock::now();
    MatchingSolve(text, pattern);
    end = std::chrono::system_clock::now();
    std::cout << "Ukkonen + Matching Statistics: " << std::chrono::duration_cast<duration_t>( end - start ).count() << " ms" << std::endl;

    return 0;
}
