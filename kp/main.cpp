#include <iostream>
#include <fstream>
#include <algorithm>
#include "classifier.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Too few arguments\n";
        exit(1);
    }

    std::string mode(argv[1]);

    if (mode == "learn") {
        if (argc != 6) {
            std::cerr << "Wrong number of arguments\n";
            exit(1);
        }

        std::string input, output;

        for (int i = 2; i <= 4; i += 2) {
            std::string flag(argv[i]);
            if (flag == "--input") {
                input = argv[i + 1];
            } else if (flag == "--output") {
                output = argv[i + 1];
            } else {
                std::cerr << "Wrong flag\n";
                exit(1);
            }
        }

        // std::cout << "flags: " << input << ' ' << output << std::endl;

        std::ifstream file(input);

        if (!file.is_open()) {
            std::cerr << "Invalid file\n";
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

        try {
            model.SaveToFile(output);
        } catch (std::exception& e) {
            std::cerr << e.what() << '\n';
            exit(1);
        }
    } else if (mode == "classify") {
        if (argc != 8) {
            std::cerr << "Wrong number of arguments\n";
            exit(1);
        }

        std::string input, stats, output;

        for (int i = 2; i <= 6; i += 2) {
            std::string flag(argv[i]);
            if (flag == "--input") {
                input = argv[i + 1];
            } else if (flag == "--output") {
                output = argv[i + 1];
            } else if (flag == "--stats") {
                stats = argv[i + 1];
            } else {
                std::cerr << "Wrong flag\n";
                exit(1);
            }
        }

        TNaiveBayesClassifier model;
        try {
            model.LoadFromFile(stats);
        } catch (std::exception& e) {
            std::cerr << e.what() << '\n';
            exit(1);
        }

        uint64_t n;
        std::vector<std::pair<TWord, std::vector<TCategoryName>>> data;
        std::ifstream file(input);
        std::ofstream out(output);

        while (file >> n) {
            file >> std::ws;
            std::string question;
            std::string rawDoc;
            for (uint64_t i = 0; i < n + 1; ++i) {
                std::string line;
                std::getline(file, line);
                if (i == 0) {
                    question = line;
                }
                rawDoc += line + ' ';
            }

            std::vector<TCategoryName> predict = model.Predict(rawDoc);
            std::sort(predict.begin(), predict.end());

            out << question << '\n';
            for (size_t i = 0; i < predict.size(); ++i) {
                out << predict[i];
                if (i < predict.size() - 1) {
                    out << ',';
                }
            }
            out << '\n';
        }
    } else {
        std::cerr << "Wrong mode\n";
        exit(1);
    }

    return 0;
}
