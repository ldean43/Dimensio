#include "ast.hpp"
#include "utils.hpp"
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
    bool currOperand, nextOperand;
    
    for (size_t i = 0; i < tokens.size(); i++) {
        newTokens.push_back(tokens[i]);
        currOperand = false;
        nextOperand = false;
        
        if (i < tokens.size() - 1) {
            // Number can be followed by var, paren, or command
            if (isDouble(tokens[i])) {
                currOperand = true;
                nextOperand = (isVar(tokens[i + 1]) || tokens[i + 1] == "(" || 
                              (validCommands.count(tokens[i + 1]) && tokens[i + 1] != "right|"));
            // Var, paren, command can be followed by everything else
            } else if (isVar(tokens[i]) || tokens[i] == ")" || 
                      (validCommands.count(tokens[i]) && tokens[i + 1] != "left|")) {
                currOperand = true;
                nextOperand = (isDouble(tokens[i]) || isVar(tokens[i]) || tokens[i] == "(" || 
                              (validCommands.count(tokens[i + 1]) && tokens[i + 1] != "right|"));
            }
            // If valid combination, place * between
            if (currOperand && nextOperand) {
                newTokens.push_back("*");
            }
        }
    }
    return newTokens;
}

std::vector<std::string> lex(std::string input) {
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
