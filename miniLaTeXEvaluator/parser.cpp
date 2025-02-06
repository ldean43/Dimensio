#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <cctype>
#include "ast.hpp"
#include "aux.hpp"

// Exponentiation with right associativity
std::unordered_map<std::string, int> precedence = {
    {"sin", 4}, {"cos", 4}, {"tan", 4}, {"csc", 4}, {"sec", 4}, {"cot", 4},
    {"arcsin", 4}, {"arccos", 4}, {"arctan", 4}, {"arcsec", 4}, {"arccsc", 4}, 
    {"arccot", 4}, {"sinh", 4}, {"cosh", 4}, {"tanh", 4}, {"frac", 4}, 
    {"sqrt", 4}, {"ln", 4}, {"log", 4}, {"lg", 4}, {"^", 3}, {"*", 2}, {"/", 2},
    {"+", 1}, {"-", 1}
};

std::unordered_set<std::string> trig = {
    "sin", "cos", "tan", "csc", "sec", "cot",
    "arcsin", "arccos", "arctan", "arcsec",
    "arccsc", "arccot", "sinh", "cosh", "tanh"
};

// Recursive descent parser using stacks to create ast from infix expression
class Parser {
    private:
        std::stack<Expr*> operands;
        std::stack<std::string> ops;
        std::vector<std::string>::iterator it;
        std::vector<std::string>::iterator end;
        std::string tok;

        void advance() {
            if (it != end) {
                it++;
                tok = *it;
            } else {
                throw std::runtime_error("parsing error: insufficient arguments");
            }
        }

        // Reapted code for unary functions
        Expr** popUnary () {
            Expr** e = (Expr**) malloc(sizeof(Expr*));
            if (operands.size() < 1) {
                throw std::runtime_error("parsing error: insufficent arguments");
            }
            *e = operands.top();
            operands.pop();
            ops.pop();
            return e;
        }

        // Repeated code for binary functions and operations
        Expr** popBinary() {
            Expr** e = (Expr**) malloc(2*sizeof(Expr*));
            if (operands.size() < 2) {
                throw std::runtime_error("parsing error: insufficent arguments");
            }
            e[1] = operands.top();
            operands.pop();
            e[0] = operands.top();
            operands.pop();
            ops.pop();
            return e;
        }

        // pops functions and operators off of ops stack and onto operands
        void popOp() {
            std::string op = ops.top();
            Expr** e;

            if (op == "+" || op == "-" || op == "*" || op == "/" || op == "^") {
                e = popBinary();
                operands.push(new Op(op[0], e[0], e[1]));
            } else if (trig.count(op)) {
                e = popUnary();
                operands.push(new Trig(op, e[0]));
            } else if (op == "frac") {
                e = popBinary();
                operands.push(new Frac(e[0], e[1]));
            } else if (op == "sqrt") {
                e = popBinary();
                operands.push(new Sqrt(e[0], e[1]));
            } else if (op == "ln") {
                e = popUnary();
                operands.push(new Ln(e[0]));
            } else if (op == "lg") {
                e = popUnary();
                operands.push(new Lg(e[0]));
            } else if (op == "log") {
                e = popBinary();
                operands.push(new Log(e[0], e[1]));
            }
            free(e);
        }

        void parseNum() {
            operands.push(new Num(stod(tok)));
            advance();
        }

        void parseVar() {
            operands.push(new Var(tok));
            advance();
        }

        void parseOp() {
            while (precedence.at(ops.top()) > precedence.at(tok)
            || (precedence.at(ops.top()) == precedence.at(tok)
                && tok != "^")) {
            }
        }

        void parseFrac() {
            // Push frac onto ops stack
            ops.push(tok);
            advance();
            // Check for required {, parse numerator
            if (tok != "{") { mismatchError("frac", "{"); }
            ops.push(tok);
            advance();
            parseExpr();
            // Check for matching }
            if (tok != "}") { mismatchError("frac", "}"); }
            advance();
            // Check for required {, parse demoninator
            if (tok != "{") { mismatchError("frac", "{"); }
            ops.push(tok);
            parseExpr();
            // Check for matching }
            if (tok != "}") { mismatchError("frac", "}");}
            advance();
        }

        void parseTrig() {
            // Push trig func onto ops stack
            ops.push(tok);
            advance();
            // Check for {, parse argument
            if (tok == "{") {
                ops.push(tok);
                advance();
                parseExpr();
                if (tok !=  "}") { mismatchError("trig", "}"); }
                advance();
            // Check for (, parse argument
            } else if (tok == "(") {
                ops.push(tok);
                advance();
                parseExpr();
                if (tok !=  ")") { mismatchError("trig", ")"); }
                advance();
            } else {
                throw std::runtime_error
                ("parsing error: " + ops.top() + " invalid syntax");
            }
        }

        void parseSqrt() {
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
                if (tok != "]") { mismatchError("sqrt", "]"); }
                advance();
            }
            // Check for required {, parse argument
            if (tok !=  "{") { mismatchError("sqrt", "{"); }
            ops.push(tok);
            advance();
            parseExpr();
            // Check for matching }
            if (tok !=  "}") { mismatchError("sqrt", "}"); }
            advance();
        }

        void parseLnLg() {
            // Push ln/log onto stack
            ops.push(tok);
            advance();
            // Check for required {, parse argument
            if (tok !=  "{") { mismatchError("ln/lg", "{"); }
            ops.push(tok);
            advance();
            parseExpr();
            // Check for matching }
            if (tok !=  "{") { mismatchError("ln/lg", "}"); }
            advance();
        }

        void parseLog() {
            // Push log onto stack
            ops.push(tok);
            advance();
            // Check for optional base
            if (tok != "_") { 
                operands.push(new Num(10)); // Implicit base 10 log
            } else {
                advance();
                // Check for required {, parse arbitrary base
                if (tok != "{") { mismatchError("log", "{"); }
                ops.push(tok);
                advance();
                parseExpr();
                // Check for matching }
                if (tok != "}") { mismatchError("log", "}"); }
                advance();
            }
            // Check for required {, parse argument
            if (tok != "{") { mismatchError("log", "{"); }
            ops.push(tok);
            advance();
            parseExpr();
            // Check for matching }
            if (tok != "}") { mismatchError("log", "}"); }
            advance();
        }

        void parseExpr() {
            if (it == end) {

            } else if (isDouble(tok)) {
                parseNum();
                return;
            } else if (tok == "frac") {
                parseFrac();
                return;
            } else if (trig.count(tok)) {
                parseTrig();
                return;
            } else if (tok == "sqrt") {
                parseSqrt();
                return;
            } else if (tok == "ln" || tok == "lg") {
                parseLnLg();
                return;
            } else if (tok == "log") {
                parseLog();
                return;
            } else if (tok == ")" || tok == "}" || tok == "]") {
                if (tok == ")") {
                    while (!ops.empty() && ops.top() != "(") {
                        popOp();
                    }
                    if (ops.top() != "(") {
                        throw std::runtime_error("parsing error: mismatched paren");
                    }
                } else if (tok == "}") {
                    while (!ops.empty() && ops.top() != "{") {
                        popOp();
                    }
                    if (ops.top() != "{") {
                        throw std::runtime_error("parsing error: mismatched brace");
                    }
                } else if (tok == "]") {
                    while (!ops.empty() && ops.top() != "[") {
                        popOp();
                    }
                    if (ops.top() != "[") {
                        throw std::runtime_error("parsing error: mismatched bracket");
                    }
                }
                return;
            }
        }

    public:
        Parser(std::vector<std::string> tokens) : it(tokens.begin()),
        end(tokens.end()) {}

        Expr* parse() {
            parseExpr();
        }
};