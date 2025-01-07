#pragma once

#include <string>
#include <vector>
#include <regex>

const std::string DELIM = " .,!?";

void ToLowerCase(std::string& str);
std::vector<std::string> Split(const std::string& s, const std::string& delimiters);
