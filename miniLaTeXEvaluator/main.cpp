#include "parser.hpp"
#include "lexer.hpp"
#include <iostream>

int main() {
    std::string input;
    std::vector<std::string> tokens;

    std::cout << "Enter input (Ctrl+D to end on Unix-like systems, Ctrl+Z followed by Enter to end on Windows):" << std::endl;

    while (true) {
        Expr* expr;
        std::cout << "> ";
        if (!getline(std::cin, input)) {
            break; // Exit the loop if input fails (e.g., EOF)
        }
        tokens = lex(input);
        for (const auto& token : tokens) {
            std::cout << token << "\n";
        }
        Parser parser(tokens);
        expr = parser.parse();
        Trig* trig = (Trig*) expr;
        Num* num = (Num*) trig->e;
        std::cout << num->value << "is the argument of " << trig->func << "\n";

        std::cout.flush();
    }

    return 0;
}