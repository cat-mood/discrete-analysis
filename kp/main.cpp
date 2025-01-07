#include <iostream>
#include "classifier.h"

int main() {
    std::vector<std::pair<TWord, std::vector<TCategoryName>>> training;

    uint64_t trainingCount, testCount;
    std::cin >> trainingCount >> testCount;

    for (uint64_t i = 0; i < trainingCount; ++i) {
        std::string rawCategories;
        std::string rawTraining;
        std::getline(std::cin, rawCategories);
        std::getline(std::cin, rawTraining);
        ToLowerCase(rawTraining);

        std::vector<TWord> split = Split(rawTraining, DELIM);
        std::vector<TCategoryName> categories = UIntSplit(rawCategories, DELIM);
        for (TWord& word : split) {
            training.push_back({word, categories});
        }
    }

    TNaiveBayesClassifier model;
    model.Fit(training);

    std::string str;

    while (std::getline(std::cin, str)) {
        std::vector<TCategoryName> predicted = model.Predict(str);
        
        for (auto& category : predicted) {
            std::cout << category << std::endl;
        }
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
