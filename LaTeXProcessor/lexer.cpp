#include <iostream>
#include <cstdlib>
#include "token.h"

using namespace std;

class Lexer {
private:
	string input;
	size_t currentIndex;
	vector<string> tokens;
public:
	Lexer(const string* input) :
		input(input), currentPos(0) { }

	char curr() {
		return (currentIndex < input.size()) : input[currentIndex] : '\0';
	}

	void next() {
		if (currentIndex < input.size()) {
			currentIndex++;
		}
	}

	vector<string> lex() {

	}
}

std::string nextToken(Lexer* lexer) {
	char c = lexer->curr();

	if (isspace(c)) {
		lexer->next();
		nextToken(lexer);
	}

	if (c == '\\') { // LaTeX command
		string command;
		lexer->next();
		c = lexer->curr();
		while (isalpha(c)) {
			command += c;
			lexer->next();
			c = lexer->curr();
		}
		return { TokenType::Command, command };
	}
}