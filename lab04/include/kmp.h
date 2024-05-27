#pragma once

#include <string>
#include <vector>
#include "parser.h"

std::vector<int> ZFunction(const TString& s);
std::vector<int> SPFunction(const TString& s);
std::vector<int> KMPFunction(const TString& pattern, const TString& text);
