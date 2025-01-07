#include "word_tools.h"

void ToLowerCase(std::string& str) {
    for (char& c : str) {
        c = tolower(c);
    }
}

std::vector<std::string> Split(const std::string& s, const std::string& delimiters) {
    std::regex re("[" + delimiters + "]");
    std::sregex_token_iterator first{s.begin(), s.end(), re, -1}, last; // the '-1' is what makes the regex split (-1 := what was not matched)
    std::vector<std::string> tokens{first, last};
    
    return tokens;
}
