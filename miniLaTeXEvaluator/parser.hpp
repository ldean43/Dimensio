#pragma once
#include "ast.hpp"
#include <stack>
#include <unordered_map>
#include <unordered_set>

class Parser {
    private:
        std::stack<Expr*> operands;
        std::stack<std::string> ops;
        std::vector<std::string>::iterator it, end;
        std::string tok, prevTok;
        bool parsed = false;

        static const std::unordered_map<std::string, int> precedence;
        static const std::unordered_set<std::string> trig;

        void advance();

        void match(std::string op, std::string expected);

        Expr* popUnary(std::string op);

        std::pair<Expr*, Expr*> popBinary(std::string op);

        void popOp();

        void parseNum();

        void parseVar();

        void parseOp();

        void parseFrac();

        void parseSqrt();

        void parseLog();

        void parseLnLg();

        void parseAbs();

        void parseTrig();

        void parseParens();

        void parseExpr();

    public:
        Parser(std::vector<std::string>& tokens);
        Expr* parse();
};