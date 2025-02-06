#include "aux.hpp"

bool isDouble(std::string str) {
    bool hasDecimal = false;
    
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == '.') { 
            if (hasDecimal) {
                return false;
            } else {
                hasDecimal = true;
            }
        } else if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

bool isVar(std::string str) {
    if (str.length() == 1 && isalpha(str[0])) {
        return true;
    }
    return false;
}

void mismatchError(std::string op, std::string delim) {
    if (delim == "{" || delim == "}") {
        throw std::runtime_error
        ("parsing error: " + op + " mismatched brace");
    } else if (delim == "[" || delim == "]") {
        throw std::runtime_error
        ("parsing error: " + op + " mismatched bracket");
    } else if (delim == "(" || delim == ")") {
        throw std::runtime_error
        ("parsing error: " + op + " mismatched paren");
    }
}
