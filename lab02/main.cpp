#include <iostream>
#include <string>
#include "patricia.h"

int main() {
    std::string input;
    TPatriciaTrie t;
    while (std::cin >> input) {
        if (input == "+") {
            std::string key;
            uint64_t value;
            std::cin >> key >> value;
            try {
                t.Insert({key, value});
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                continue;
            }
        } else if (input == "-") {
            std::string key;
            std::cin >> key;
            try {
                t.Erase(key);
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                continue;
            }
        } else if (input == "!") {
            std::cout << "NOT READY!!" << std::endl;
        } else {
            TPair<std::string, uint64_t> p;
            try {
                p = t.Find(input);
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                continue;
            }
            std::cout << "OK: " << p.value << std::endl;
            continue;
        }
        std::cout << "OK" << std::endl;
    }

    return 0;
}
