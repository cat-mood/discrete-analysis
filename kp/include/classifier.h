#pragma once

#include <vector>
#include <unordered_map>
#include <cstdint>
#include <string>
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include "word_tools.h"

using TWord = std::string;
using TCategoryName = uint64_t;
using TConfusionMatrix = std::vector<std::vector<uint64_t>>;

class TNaiveBayesClassifier {
private:
    using TCategory = std::unordered_map<TWord, uint64_t>;

    std::unordered_map<TCategoryName, TCategory> categoriesWeights;
    double beta = 0.5;
public:
    TNaiveBayesClassifier() {}
    void Fit(const std::vector<std::pair<TWord, std::vector<TCategoryName>>>& data);
    std::vector<TCategoryName> Predict(std::string& str);
    void SaveToFile(const std::string& filename) const;
    void LoadFromFile(const std::string& filename);

    std::unordered_map<TCategoryName, TCategory> GetCategoriesWeights() const {
        return categoriesWeights;
    }

    uint64_t GetCategoriesNumber() const {
        return categoriesWeights.size();
    }

    static double Accuracy(const TConfusionMatrix& matrix);

    static double Precision(const TConfusionMatrix& matrix, TCategoryName category);

    static double Recall(const TConfusionMatrix& matrix, TCategoryName category);

    static double F1(const TConfusionMatrix& matrix, TCategoryName category);
};
