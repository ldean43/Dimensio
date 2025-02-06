#include <iostream>
#include "ast.hpp"

int main() {
    std::string input;
    std::vector<std::string> tokens;
    std::cin.clear();
    while(std::cin) {
        getline(std::cin, input);
        tokens = lex(input);
        for (auto i = tokens.begin(); i != tokens.end(); i++) {
            std::cout << *i << "\n";
        }
        std::cout.flush();
    }
    return 0;
}