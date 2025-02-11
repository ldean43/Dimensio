#include "ast.hpp"
#include "utils.hpp"
#include "lexer.hpp"
#include <stdexcept>
#include <unordered_set>

const static std::unordered_set<std::string> validCommands = {
    "sin", "cos", "tan", "csc", "sec", "cot",
    "arcsin", "arccos", "arctan", "arcsec",
    "arccsc", "arccot", "sinh", "cosh", "tanh",
    "frac", "sqrt", "ln", "log", "lg", "right|", 
    "left|"
};

const static std::unordered_set<char> validSymbols = {
    '+', '-', '/', '^', '=', '(', ')', '{', '}', '[', ']', '_'
};

// Retokenizes for cases with implicit multiplication i.e. 5(2), 5x
std::vector<std::string> implicitMult(std::vector<std::string> tokens) {
    std::vector<std::string> newTokens;
    auto it = tokens.begin();
    std::string tok = *it;
    std::string prevTok = nullptr;
    
    while(it != (tokens.end() - 1)) {
        newTokens.push_back(tok);
        if (isDouble(tok)) {
            tok = *(it++);
            if (tok == "{") {
                while (it != (tokens.end() - 1) && tok == "{") {
                    tok = *(it++);
                }
            }
        }
    }
    return newTokens;
}

std::vector<std::string> Lexer::lexHelper() {
    std::vector<std::string> tokens;
    std::string command;
    size_t currIndex = 0;

    auto curr = [&]() -> char {
        return (currIndex < input.size()) ? input[currIndex] : '\0';
    };
    auto next = [&]() {
        if (currIndex < input.size()) currIndex++;
    };
    while (currIndex < input.size()) {
        char c = curr();

        // Handle whitespace
        if (isspace(c)) {
            next();
            continue;
        }
        // Commands
        if (c == '\\') {
            next();
            c = curr();
            while (isalpha(c) || c == '|') {
                command += c;
                next();
                c = curr();
            }
            // Check if command is valid
            if (validCommands.count(command)) {
                tokens.push_back(command);
                command = "";
                continue;
            } else {
                throw std::runtime_error
                ("lexing error: invalid command " + command);
            }
        }
        /* Commands will typically be followed by a symbol, symbol logic
        immediately after command logic to optimize time */
        if (validSymbols.count(c)) {
            tokens.push_back(std::string(1, c));
            next();
            continue;
        }
        // Variables
        if (isalpha(c)) {
            tokens.push_back(std::string(1,c));
            next();
            continue;
        }
        // Numbers
        if (std::isdigit(c) || c == '.') {
            std::string number(1,c);
            bool hasDecimal = c == '.';

            next();
            c = curr();
            while (std::isdigit(c) || c == '.') {
                // Check for two decimals
                if (c == '.') { 
                    if (hasDecimal) {
                        throw std::runtime_error("lexing error: invalid input");
                    } else {
                        hasDecimal = true;
                    }
                }
                // Append digit to number
                number += c;
                next();
                c = curr();
            }
            tokens.push_back(number);
        }
    }
    return implicitMult(tokens);
}
