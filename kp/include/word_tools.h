#pragma once

#include <string>
#include <vector>
#include <regex>
#include <cstdint>

const std::string DELIM = " .,!?";

void ToLowerCase(std::string& str);
std::vector<std::string> Split(const std::string& s, const std::string& delimiters);
std::vector<uint64_t> UIntSplit(const std::string& s, const std::string& delimeters);
