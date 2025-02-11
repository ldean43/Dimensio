#include "ast.hpp"
#include "utils.hpp"
#include "parser.hpp"
#include <stdexcept>

// Recursive descent parser using stacks to create ast from infix expression

void Parser::advance() {
    if (it != (end - 1)) {
        it++;
        prevTok = tok;
        tok = *it;
    }
    return;
}

void Parser::match(std::string op, std::string expected) {
    if (tok != expected) {
        throw std::runtime_error("parsing error: expected " + expected + " after " + prevTok + " in " + op);
    }
}

// Reapted code for unary functions
Expr* Parser::popUnary (std::string op) {
    if (operands.size() < 1) {
        throw std::runtime_error("parsing error: insufficent arguments for " + op);
    }
    Expr* e = operands.top();
    operands.pop();
    ops.pop();
    return e;
}

// Repeated code for binary functions and operations
std::pair<Expr*, Expr*> Parser::popBinary(std::string op) {
    if (operands.size() < 2) {
        throw std::runtime_error("parsing error: insufficent arguments for " + op);
    }
    Expr* e2 = operands.top();
    operands.pop();
    Expr* e1 = operands.top();
    operands.pop();
    ops.pop();
    return {e1, e2};
}

// pops functions and operators off of ops stack and onto operands
void Parser::popOp() {
    std::string op = ops.top();

    if (op == "+" || op == "-" || op == "*" || op == "/" || op == "^") {
        auto e = popBinary(op);
        operands.push(new Op(op[0], e.first, e.second));
    } else if (op == "frac") {
        auto e = popBinary(op);
        operands.push(new Frac(e.first, e.second));
    } else if (op == "sqrt") {
        auto e = popBinary(op);
        operands.push(new Sqrt(e.first, e.second));
    } else if (op == "log") {
        auto e = popBinary(op);
        operands.push(new Log(e.first, e.second));
    } else if (op == "ln") {
        Expr* e = popUnary(op);
        operands.push(new Ln(e));
    } else if (op == "lg") {
        Expr* e = popUnary(op);
        operands.push(new Lg(e));
    } else if (trig.count(op)) {
        Expr* e = popUnary(op);
        operands.push(new Trig(op, e));
    }
    return;
}

void Parser::parseNum() {
    operands.push(new Num(stod(tok)));
    advance();
    return;
}

void Parser::parseVar() {
    operands.push(new Var(tok));
    advance();
    return;
}

void Parser::parseOp() {
    while (!ops.empty() &&
            (precedence.at(ops.top()) > precedence.at(tok) ||
            (precedence.at(ops.top()) == precedence.at(tok) && tok != "^"))) {
        popOp();
    }
    ops.push(tok);
    advance();
    return;
}

void Parser::parseFrac() {
    // Push frac onto ops stack
    ops.push(tok);
    advance();
    // Check for required {, parse numerator
    match("frac", "{");
    ops.push(tok);
    advance();
    parseExpr();
    // Check for matching }
    match("frac", "}");
    advance();
    // Check for required {, parse demoninator
    match("frac", "{");
    ops.push(tok);
    parseExpr();
    // Check for matching }
    match("frac", "}");
    advance();
    return;
}

void Parser::parseSqrt() {
    // Push sqrt onto ops stack
    ops.push(tok);
    advance();
    // Check for optional root
    if (tok != "[") {
        operands.push(new Num(2)); // if no custom root, choose 2
    // Check for [, parse arbitrary root
    } else {
        ops.push(tok);
        advance();
        parseExpr();
        match("sqrt", "]");
        advance();
    }
    // Check for required {, parse argument
    match("sqrt", "{");
    ops.push(tok);
    advance();
    parseExpr();
    // Check for matching }
    match("sqrt", "}");
    advance();
    return;
}

void Parser::parseLog() {
    // Push log onto stack
    ops.push(tok);
    advance();
    // Check for optional base
    if (tok != "_") { 
        operands.push(new Num(10)); // Implicit base 10 log
    } else {
        advance();
        // Check for required {, parse arbitrary base
        match("log", "{");
        ops.push(tok);
        advance();
        parseExpr();
        // Check for matching }
        match("log", "}");
        advance();
    }
    // Check for required {, parse argument
    match("log", "{");
    ops.push(tok);
    advance();
    parseExpr();
    // Check for matching }
    match("log", "}");
    advance();
    return;
}

void Parser::parseLnLg() {
    std::string func = tok;
    // Push ln/log onto stack
    ops.push(tok);
    advance();
    // Check for required {, parse argument
    match(func, "{");
    ops.push(tok);
    advance();
    parseExpr();
    // Check for matching }
    match(func, "}");
    advance();
    return;
}

void Parser::parseAbs() {
    // Push right| onto stack
    ops.push(tok);
    advance();
    parseExpr();
    // Check for required left|
    match("absolute value", "left|");
    advance();
    return;
}

void Parser::parseTrig() {
    std::string func = tok;
    // Push trig func onto ops stack
    ops.push(func);
    advance();
    // Check for {, parse argument
    if (tok == "{") {
        ops.push(tok);
        advance();
        parseExpr();
        match(func, "}");
        advance();
    // Check for (, parse argument
    } else if (tok == "(") {
        ops.push(tok);
        advance();
        parseExpr();
        match(func, ")");
        parseParens();
        advance();
    } else {
        throw std::runtime_error
        ("parsing error: expected { or ( after " + func);
    }
    return;
}

void Parser::parseParens() {
    if (tok == ")") {
        while (!ops.empty() && ops.top() != "(") {
            popOp();
        }
        if (ops.top() != "(") {
            throw std::runtime_error("parsing error: mismatched parens");
        }
        ops.pop();
    } else if (tok == "}") {
        while (!ops.empty() && ops.top() != "{") {
            popOp();
        }
        if (ops.top() != "{") {
            throw std::runtime_error("parsing error: mismatched braces");
        }
        ops.pop();
    } else if (tok == "]") {
        while (!ops.empty() && ops.top() != "[") {
            popOp();
        }
        if (ops.top() != "[") {
            throw std::runtime_error("parsing error: mismatched brackets");
        }
        ops.pop();
    } else if (tok == "right|") {
        while (!ops.empty() && ops.top() != "left|") {
            popOp();
        }
        if (ops.top() != "left|") {
            throw std::runtime_error("parsing error: mismatched left|");
        }
        ops.pop();
    }
    return;
}

void Parser::parseExpr() {
    if (isDouble(tok)) {
        parseNum();
    } else if (isVar(tok)) {
        parseVar();
    } else if (tok == "+" || tok == "-" || tok == "*" || tok == "/" || tok == "^") {
        parseOp();
    } else if (tok == "frac") {
        parseFrac();
    } else if (tok == "sqrt") {
        parseSqrt();
    } else if (tok == "log") {
        parseLog();
    } else if (tok == "ln" || tok == "lg") {
        parseLnLg();
    } else if (trig.count(tok)) {
        parseTrig();
    } else if (tok == ")" || tok == "}" || tok == "]" || tok == "left|") {
        parseParens();
    }
    return;
}

Parser::Parser(std::vector<std::string>& tokens) : it(tokens.begin()),
end(tokens.end()) {
    tok = *it;
}

Expr* Parser::parse() {
    if (!parsed) {
        parseExpr();
        while (!ops.empty()) {
            popOp();
        }
        parsed = true;
    }
    return operands.top();
}

// Exponentiation with right associativity
const std::unordered_map<std::string, int> Parser::precedence = {
    {"sin", 4}, {"cos", 4}, {"tan", 4}, {"csc", 4}, {"sec", 4}, {"cot", 4},
    {"arcsin", 4}, {"arccos", 4}, {"arctan", 4}, {"arcsec", 4}, {"arccsc", 4}, 
    {"arccot", 4}, {"sinh", 4}, {"cosh", 4}, {"tanh", 4}, {"frac", 4}, 
    {"sqrt", 4}, {"ln", 4}, {"log", 4}, {"lg", 4}, {"^", 3}, {"*", 2}, {"/", 2},
    {"+", 1}, {"-", 1}
};

const std::unordered_set<std::string> Parser::trig = {
    "sin", "cos", "tan", "csc", "sec", "cot",
    "arcsin", "arccos", "arctan", "arcsec",
    "arccsc", "arccot", "sinh", "cosh", "tanh"
};
