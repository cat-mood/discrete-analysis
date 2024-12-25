#include <iostream>
#include <cstdint>
#include <unordered_map>
#include <string>
#include <vector>
#include <regex>

using TWord = std::string;
using TCategoryName = uint8_t;
const std::string DELIM = " .,!?";

void ToLowerCase(std::string& str) {
    for (char& c : str) {
        c = tolower(c);
    }
}

std::vector<std::string> Split(const std::string& s, const std::string& delimiters) {
    std::regex re("[" + delimiters + "]");
    std::sregex_token_iterator first{s.begin(), s.end(), re, -1}, last; //the '-1' is what makes the regex split (-1 := what was not matched)
    std::vector<std::string> tokens{first, last};
    
    return tokens;
}

class TNaiveBayesClassifier {
private:
    using TCategory = std::unordered_map<TWord, uint64_t>;

    std::unordered_map<TCategoryName, TCategory> categoriesWeights;
public:
    TNaiveBayesClassifier() {}
    void Fit(const std::vector<std::pair<TWord, TCategoryName>>& data);
    TCategoryName Predict(std::string& str);

    std::unordered_map<TCategoryName, TCategory> GetCategoriesWeights() const {
        return categoriesWeights;
    }
};

void TNaiveBayesClassifier::Fit(const std::vector<std::pair<TWord, TCategoryName>>& training) {
    for (auto& word : training) {
        TCategory& category = categoriesWeights[word.second];
        
        if (!category.contains(word.first)) {
            category[word.first] = 0;
        }
        ++category[word.first];
    }
}

TCategoryName TNaiveBayesClassifier::Predict(std::string& str) {
    ToLowerCase(str);
    std::vector<TWord> split = Split(str, DELIM);
    std::unordered_map<TCategoryName, double> probabilities;

    uint64_t sumCategories = 0;

    for (auto& category : categoriesWeights) {
        sumCategories += category.second.size();
    }

    for (auto& category : categoriesWeights) {
        double probability = 0;

        for (TWord& word : split) {
            if (!category.second.contains(word)) {
                probability += 1;
            } else {
                probability += category.second[word] + 1;
            }
        }
        // (category.second.size() / sumCategories) * probability / category.second.size()
        probabilities[category.first] = probability / sumCategories;
    }

    double max = 0;
    TCategoryName maxCategoryName;

    for (auto& category : probabilities) {
        if (max < category.second) {
            max = category.second;
            maxCategoryName = category.first;
        }
    }

    return maxCategoryName;
}

int main() {
    std::vector<std::pair<TWord, TCategoryName>> training;

    uint64_t trainingCount, testCount;
    std::cin >> trainingCount >> testCount;

    for (uint64_t i = 0; i < trainingCount; ++i) {
        TCategoryName category;
        std::string rawTraining;
        std::cin >> category;
        std::cin >> std::ws;
        std::getline(std::cin, rawTraining);
        ToLowerCase(rawTraining);

        std::vector<TWord> split = Split(rawTraining, DELIM);
        for (TWord& word : split) {
            training.push_back({word, category});
        }
    }

    TNaiveBayesClassifier model;
    model.Fit(training);

    std::string str;

    while (std::getline(std::cin, str)) {
        std::cout << model.Predict(str) << std::endl;
    }

    // auto weights = model.GetCategoriesWeights();

    // for (auto& weight : weights) {
    //     std::cout << weight.first << std::endl;
    //     for (auto& el : weight.second) {
    //         std::cout << el.first << ' ' << el.second << std::endl;
    //     }
    // }

    return 0;
}
