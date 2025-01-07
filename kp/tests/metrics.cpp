#include "classifier.h"
#include <gtest/gtest.h>
#include <fstream>
#include <algorithm>

// #define DEBUG_OUTPUT 1
// #define METRICS_OUTPUT 1

const double ACCURACY_THRESHOLD = 0.6;
const double PRECISION_THRESHOLD = 0.5;
const double RECALL_THRESHOLD = 0.5;
const double F1_THRESHOLD = 0.5;

const std::string FILES_PATH = "../tests/files/";

TConfusionMatrix ReadTestFile(std::ifstream& testFile) {
    TNaiveBayesClassifier model;
    std::vector<std::pair<TWord, std::vector<TCategoryName>>> training;
    uint64_t trainingCount, testCount;
    testFile >> trainingCount >> testCount;
    #ifdef DEBUG_OUTPUT
        std::cout << "trainingCount, testCount: " << trainingCount << ", " << testCount << std::endl;
    #endif

    for (uint64_t i = 0; i < trainingCount; ++i) {
        std::string rawCategories;
        std::string rawTraining;
        testFile >> std::ws;
        std::getline(testFile, rawCategories);
        std::getline(testFile, rawTraining);
        #ifdef DEBUG_OUTPUT
            std::cout << "rawCategories: " << rawCategories<< std::endl;
            std::cout << "rawTraining: " << rawTraining << std::endl;
        #endif
        ToLowerCase(rawTraining);

        std::vector<TWord> split = Split(rawTraining, DELIM);
        std::vector<TCategoryName> categories = UIntSplit(rawCategories, DELIM);

        for (TWord& word : split) {
            training.push_back({word, categories});
        }
    }

    model.Fit(training);

    std::string str;
    std::string rawCategories;

    // +1 for undefined category
    TConfusionMatrix confusionMatrix(model.GetCategoriesNumber() + 1, std::vector<uint64_t>(model.GetCategoriesNumber() + 1, 0));

    for (uint64_t i = 0; i < testCount; ++i) {
        std::getline(testFile, rawCategories);
        std::getline(testFile, str);
        std::vector<TCategoryName> trueCategories = UIntSplit(rawCategories, DELIM);
        #ifdef DEBUG_OUTPUT
            std::cout << "trueCategories: " << rawCategories << std::endl;
            std::cout << "str: " << str << std::endl;
        #endif

        std::vector<TCategoryName> predicted = model.Predict(str);


        std::sort(predicted.begin(), predicted.end());
        std::sort(trueCategories.begin(), trueCategories.end());

        for (uint64_t j = 0; j < std::max(predicted.size(), trueCategories.size()); ++j) {
            uint64_t predictedCategory = confusionMatrix.size() - 1;
            uint64_t trueCategory = confusionMatrix.size() - 1;

            if (j < predicted.size()) {
                predictedCategory = predicted[j];
            }
            if (j < trueCategories.size()) {
                trueCategory = trueCategories[j];
            }

            ++confusionMatrix[trueCategory][predictedCategory];

            #ifdef DEBUG_OUTPUT
                std::cout << "predictedCategory: " << predictedCategory << std::endl;
            #endif
        }

    }

    return confusionMatrix;
}

TEST(metrics, small_many_test) {
    std::ifstream testFile(FILES_PATH + "small_test.txt");

    ASSERT_TRUE(testFile.is_open());

    TConfusionMatrix confusionMatrix = ReadTestFile(testFile);

    #ifdef METRICS_OUTPUT
        std::cout << "Confusion Matrix: " << std::endl;
        for (uint64_t i = 0; i < confusionMatrix.size(); ++i) {
            for (uint64_t j = 0; j < confusionMatrix.size(); ++j) {
                std::cout << confusionMatrix[i][j] << ' ';
            }
            std::cout << std::endl;
        }
    #endif

    ASSERT_GE(TNaiveBayesClassifier::Accuracy(confusionMatrix), ACCURACY_THRESHOLD);

 for (uint64_t i = 0; i < confusionMatrix.size() - 1; ++i) {
        ASSERT_GE(TNaiveBayesClassifier::Precision(confusionMatrix, i), PRECISION_THRESHOLD);
        ASSERT_GE(TNaiveBayesClassifier::Recall(confusionMatrix, i), RECALL_THRESHOLD);
        ASSERT_GE(TNaiveBayesClassifier::F1(confusionMatrix, i), F1_THRESHOLD);
    }
}

TEST(metrics, slight_test) {
    std::ifstream testFile(FILES_PATH + "slight_test.txt", std::ios::in);

    ASSERT_TRUE(testFile.is_open());

    TConfusionMatrix confusionMatrix = ReadTestFile(testFile);

    #ifdef METRICS_OUTPUT
        std::cout << "Confusion Matrix: " << std::endl;
        for (uint64_t i = 0; i < confusionMatrix.size(); ++i) {
            for (uint64_t j = 0; j < confusionMatrix.size(); ++j) {
                std::cout << confusionMatrix[i][j] << ' ';
            }
            std::cout << std::endl;
        }
    #endif

    #ifdef METRICS_OUTPUT
        std::cout << "Accuracy: " << TNaiveBayesClassifier::Accuracy(confusionMatrix) << std::endl;
    #endif
    ASSERT_GE(TNaiveBayesClassifier::Accuracy(confusionMatrix), ACCURACY_THRESHOLD);

 for (uint64_t i = 0; i < confusionMatrix.size() - 1; ++i) {
        #ifdef METRICS_OUTPUT
            std::cout << "i: " << i << std::endl;
            std::cout << "Precision: " << TNaiveBayesClassifier::Precision(confusionMatrix, i) << std::endl;
            std::cout << "Recall: " << TNaiveBayesClassifier::Recall(confusionMatrix, i) << std::endl;
            std::cout << "F1: " << TNaiveBayesClassifier::F1(confusionMatrix, i) << std::endl;
        #endif
        ASSERT_GE(TNaiveBayesClassifier::Precision(confusionMatrix, i), PRECISION_THRESHOLD);
        ASSERT_GE(TNaiveBayesClassifier::Recall(confusionMatrix, i), RECALL_THRESHOLD);
        ASSERT_GE(TNaiveBayesClassifier::F1(confusionMatrix, i), F1_THRESHOLD);
    }
}

// TEST(metrics, big_test) {
//     std::ifstream testFile(FILES_PATH + "big_test.txt", std::ios::in);

//     ASSERT_TRUE(testFile.is_open());

//     TConfusionMatrix confusionMatrix = ReadTestFile(testFile);
//     #ifdef METRICS_OUTPUT
//         std::cout << "Confusion Matrix: " << std::endl;
//         for (uint64_t i = 0; i < confusionMatrix.size(); ++i) {
//             for (uint64_t j = 0; j < confusionMatrix.size(); ++j) {
//                 std::cout << confusionMatrix[i][j] << ' ';
//             }
//             std::cout << std::endl;
//         }
//     #endif

//     ASSERT_GE(TNaiveBayesClassifier::Accuracy(confusionMatrix), ACCURACY_THRESHOLD);

//     for (uint64_t i = 0; i < confusionMatrix.size(); ++i) {
//         #ifdef DEBUG_OUTPUT
//             std::cout << "i: " << i << std::endl;
//         #endif
//         ASSERT_GE(TNaiveBayesClassifier::Precision(confusionMatrix, i), PRECISION_THRESHOLD);
//         ASSERT_GE(TNaiveBayesClassifier::Recall(confusionMatrix, i), RECALL_THRESHOLD);
//         ASSERT_GE(TNaiveBayesClassifier::F1(confusionMatrix, i), F1_THRESHOLD);
//     }
// }
