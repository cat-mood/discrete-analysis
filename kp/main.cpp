#include <iostream>
#include "classifier.h"

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
