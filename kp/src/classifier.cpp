#include "classifier.h"

void TNaiveBayesClassifier::Fit(const std::vector<std::pair<TWord, std::vector<TCategoryName>>>& training) {
    for (auto& word : training) {
        for (auto& category : word.second) {
            TCategory& weightCategory = categoriesWeights[category];
            
            if (!weightCategory.contains(word.first)) {
                weightCategory[word.first] = 0;
            }
            ++weightCategory[word.first];
        }
    }

    double maxSize = 0;

    for (auto& category : categoriesWeights) {
        maxSize = (category.second.size() > maxSize) ? category.second.size() : maxSize;
    }

    beta += 1 / maxSize;
}

std::vector<TCategoryName> TNaiveBayesClassifier::Predict(std::string& str) {
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
        probabilities[category.first] = probability / (sumCategories);
    }

    std::vector<TCategoryName> categoryNames;
    double maxProbability = 0;


    for (auto& category : probabilities) {
        maxProbability = (category.second > maxProbability) ? category.second : maxProbability;
    }

    for (auto& category : probabilities) {
        if (category.second / maxProbability > beta) {
            categoryNames.push_back(category.first);
        }
    }

    return categoryNames;
}

void TNaiveBayesClassifier::SaveToFile(const std::string& filename) const {
    std::ofstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Invalid file");
    }

    file << categoriesWeights.size() << '\n';

    for (auto& w : categoriesWeights) {
        file << w.first << ' ' << w.second.size() << '\n';

        for (auto& category : w.second) {
            file << category.first << ' ' << category.second << '\n';
        }
    }

    file << beta << '\n';
}

void TNaiveBayesClassifier::LoadFromFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Invalid file");
    }

    std::unordered_map<TCategoryName, TCategory> categoriesWeights;

    size_t categoriesWeightsSize;
    file >> categoriesWeightsSize;

    for (uint64_t i = 0; i < categoriesWeightsSize; ++i) {
        TCategoryName name;
        file >> name;

        size_t categorySize;
        file >> categorySize;

        if (file.peek() == EOF) {
            throw std::runtime_error("Bad file");
        }

        for (uint64_t j = 0; j < categorySize; ++j) {
            TWord word;
            uint64_t count;
            file >> word >> count;
            if (file.peek() == EOF) {
                throw std::runtime_error("Bad file");
            }
            categoriesWeights[name][word] = count;
        }
    }

    file >> beta;

    this->categoriesWeights = std::move(categoriesWeights);
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
        if (i != category) {
            falseCategories += matrix[i][category];
        }
    }
    return (trueCategories) / static_cast<double>(trueCategories + falseCategories);
}

double TNaiveBayesClassifier::Recall(const TConfusionMatrix& matrix, TCategoryName category) {
    uint64_t trueCategories = matrix[category][category];
    uint64_t falseCategories = 0;
    for (uint64_t i = 0; i < matrix.size(); ++i) {
        if (i != category) {
            falseCategories += matrix[category][i];
        }
    }
    return (trueCategories) / static_cast<double>(trueCategories + falseCategories);
}

double TNaiveBayesClassifier::F1(const TConfusionMatrix& matrix, TCategoryName category) {
    double precision = Precision(matrix, category);
    double recall = Recall(matrix, category);

    return 2 * precision * recall / (precision + recall);
}
