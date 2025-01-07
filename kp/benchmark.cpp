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
    TCategoryName trueCategory;
    /*
        TN  FP
        FN  TP
    */
    TConfusionMatrix confusionMatrix(model.GetClassesNumber(), std::vector<double>(model.GetClassesNumber(), 0));

    while (std::cin >> trueCategory, std::cin >> std::ws, std::getline(std::cin, str)) {
        TCategoryName predicted = model.Predict(str);
        confusionMatrix[trueCategory][predicted] += 1;
        std::cout << "Predicted: " << predicted << std::endl;
    }

    std::cout << "Accuracy: " << TNaiveBayesClassifier::Accuracy(confusionMatrix) << std::endl;
    std::cout << "Precision: " << TNaiveBayesClassifier::Precision(confusionMatrix) << std::endl;
    std::cout << "Recall: " << TNaiveBayesClassifier::Recall(confusionMatrix) << std::endl;
    std::cout << "F1: " << TNaiveBayesClassifier::F1(confusionMatrix) << std::endl;


    return 0;
}
