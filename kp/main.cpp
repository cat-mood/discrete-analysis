#include <iostream>
#include <fstream>
#include "classifier.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Too few arguments";
        exit(1);
    }

    std::string mode(argv[1]);

    if (mode == "learn") {
        if (argc != 6) {
            std::cerr << "Wrong number of arguments" << std::endl;
            exit(1);
        }

        std::string input, output;

        for (int i = 2; i < 4; i += 2) {
            std::string flag(argv[2]);
            if (flag == "--input") {
                input = argv[3];
            } else if (flag == "--output") {
                output = argv[5];
            } else {
                std::cerr << "Wrong flag" << std::endl;
                exit(1);
            }
        }

        std::ifstream file(input);

        if (!file.is_open()) {
            std::cerr << "Invalid file" << std::endl;
            exit(1);
        }

        uint64_t n;
        std::vector<std::pair<TWord, std::vector<TCategoryName>>> data;

        while (file >> n) {
            file >> std::ws;
            std::string rawTags;
            std::getline(file, rawTags);
            std::vector<TCategoryName> tags = UIntSplit(rawTags, DELIM);
            std::string rawDoc;
            for (uint64_t i = 0; i < n + 1; ++i) {
                std::string line;
                std::getline(file, line);
                rawDoc += line + ' ';
            }

            std::vector<std::string> doc = Split(rawDoc, DELIM);

            for (auto& word : doc) {
                data.push_back({word, tags});
            }
        }

        TNaiveBayesClassifier model;
        model.Fit(data);
    } else if (mode == "classify") {
        
    }

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
