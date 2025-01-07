#include "classifier.h"
#include <gtest/gtest.h>
#include <fstream>

// #define DEBUG_OUTPUT 1
#define METRICS_OUTPUT 1

const double ACCURACY_TRESHOLD = 0.7;
const double PRECISION_TRESHOLD = 0.7;
const double RECALL_TRESHOLD = 0.6;
const double F1_TRESHOLD = 0.7;

const std::string FILES_PATH = "../tests/files/";

TConfusionMatrix ReadTestFile(std::ifstream& testFile) {
    TNaiveBayesClassifier model;
    std::vector<std::pair<TWord, TCategoryName>> training;
    uint64_t trainingCount, testCount;
    testFile >> trainingCount >> testCount;
    #ifdef DEBUG_OUTPUT
        std::cout << "trainingCount, testCount: " << trainingCount << ", " << testCount << std::endl;
    #endif

    for (uint64_t i = 0; i < trainingCount; ++i) {
        TCategoryName category;
        std::string rawTraining;
        testFile >> category;
        testFile >> std::ws;
        std::getline(testFile, rawTraining);
        #ifdef DEBUG_OUTPUT
            std::cout << "category: " << category << std::endl;
            std::cout << "rawTraining: " << rawTraining << std::endl;
        #endif
        ToLowerCase(rawTraining);

        std::vector<TWord> split = Split(rawTraining, DELIM);
        for (TWord& word : split) {
            training.push_back({word, category});
        }
    }

    model.Fit(training);

    std::string str;
    TCategoryName trueCategory;
    TConfusionMatrix confusionMatrix(model.GetCategoriesNumber(), std::vector<uint64_t>(model.GetCategoriesNumber(), 0));

    for (uint64_t i = 0; i < testCount; ++i) {
        testFile >> trueCategory;
        testFile >> std::ws;
        std::getline(testFile, str);
        #ifdef DEBUG_OUTPUT
            std::cout << "trueCategory: " << trueCategory << std::endl;
            std::cout << "str: " << str << std::endl;
        #endif

        TCategoryName predicted = model.Predict(str);
        confusionMatrix[trueCategory][predicted] += 1;
        #ifdef DEBUG_OUTPUT
            std::cout << "predictedCategory: " << predicted << std::endl;
        #endif
    }

    return confusionMatrix;
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
    ASSERT_GE(TNaiveBayesClassifier::Accuracy(confusionMatrix), ACCURACY_TRESHOLD);

    for (uint64_t i = 0; i < confusionMatrix.size(); ++i) {
        #ifdef METRICS_OUTPUT
            std::cout << "i: " << i << std::endl;
            std::cout << "Precision: " << TNaiveBayesClassifier::Precision(confusionMatrix, i) << std::endl;
            std::cout << "Recall: " << TNaiveBayesClassifier::Recall(confusionMatrix, i) << std::endl;
            std::cout << "F1: " << TNaiveBayesClassifier::F1(confusionMatrix, i) << std::endl;
        #endif
        ASSERT_GE(TNaiveBayesClassifier::Precision(confusionMatrix, i), PRECISION_TRESHOLD);
        ASSERT_GE(TNaiveBayesClassifier::Recall(confusionMatrix, i), RECALL_TRESHOLD);
        ASSERT_GE(TNaiveBayesClassifier::F1(confusionMatrix, i), F1_TRESHOLD);
    }
}

TEST(metrics, big_test) {
    std::ifstream testFile(FILES_PATH + "big_test.txt", std::ios::in);

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

    ASSERT_GE(TNaiveBayesClassifier::Accuracy(confusionMatrix), ACCURACY_TRESHOLD);

    for (uint64_t i = 0; i < confusionMatrix.size(); ++i) {
        #ifdef DEBUG_OUTPUT
            std::cout << "i: " << i << std::endl;
        #endif
        ASSERT_GE(TNaiveBayesClassifier::Precision(confusionMatrix, i), PRECISION_TRESHOLD);
        ASSERT_GE(TNaiveBayesClassifier::Recall(confusionMatrix, i), RECALL_TRESHOLD);
        ASSERT_GE(TNaiveBayesClassifier::F1(confusionMatrix, i), F1_TRESHOLD);
    }
}
