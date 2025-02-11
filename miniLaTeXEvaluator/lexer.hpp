#include <string>
#include <vector>
#include <unordered_set>

class Lexer {
    private:
        std::vector<std::string>::iterator it, end;
        std::string tok;
        const static std::unordered_set<std::string> validCommands;
        const static std::unordered_set<char> validSymbols;

        void advance();

        std::vector<std::string> implicitMult();

        std::vector<std::string> lexHelper();
    public:
        Lexer(std::string& input);
        std::vector<std::string> lex();
}