#include "classifier.h"

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

double TNaiveBayesClassifier::Accuracy(const TConfusionMatrix& matrix) {
    uint64_t trueCategories = 0;
    uint64_t allCategories = 0;
    for (uint64_t i = 0; i < matrix.size(); ++i) {
        for (uint64_t j = 0; j < matrix.size(); ++j) {
            if (i == j) {
                trueCategories += matrix[i][j];
            }
            allCategories += matrix[i][j];
        }
    }
    return (trueCategories) / static_cast<double>(allCategories);
}

double TNaiveBayesClassifier::Precision(const TConfusionMatrix& matrix, TCategoryName category) {
    uint64_t trueCategories = matrix[category][category];
    uint64_t falseCategories = 0;
    for (uint64_t i = 0; i < matrix.size(); ++i) {
        falseCategories += matrix[i][category];
    }
    return (trueCategories) / static_cast<double>(trueCategories + falseCategories);
}

double TNaiveBayesClassifier::Recall(const TConfusionMatrix& matrix, TCategoryName category) {
    uint64_t trueCategories = matrix[category][category];
    uint64_t falseCategories = 0;
    for (uint64_t i = 0; i < matrix.size(); ++i) {
        falseCategories += matrix[category][i];
    }
    return (trueCategories) / static_cast<double>(trueCategories + falseCategories);
}

double TNaiveBayesClassifier::F1(const TConfusionMatrix& matrix, TCategoryName category) {
    double precision = Precision(matrix, category);
    double recall = Recall(matrix, category);

    return 2 * precision * recall / (precision + recall);
}
