#ifndef LEXER_H
#define LEXER_H

#include <queue>
#include <string>
#include <fstream>
#include <streambuf>
#include <cstdio>
#include <cmath>
#include <cctype>
#include <unordered_map> 
#include <vector>

// DEBUG
//#include <iostream>
// END DEBUG

constexpr uint LXhashPairs(int x, int y){
		return pow(2, 16) * x + y;
}


class Lexer {

	enum State {NoState, inWord, inNum, inSpace, inOp};
	enum characterType { NoType, Alpha, Numeric, Special, Space = ' ', Tab = '\t', NewLine = '\n'};

	std::vector<std::string> doubleOps = {"==", "<=", ">=", "!=", "+=", "-=", "*=", "/=", "%=", "++", "--", "**", "//" ">>", "<<", "&&", "||"};
	std::unordered_map<std::string, int> doubleOpsMap;

	std::queue<std::string> parseString(std::string input);
	void throwError(std::string errorMessage);

	std::string appendToken(std::queue<std::string> &tokens, std::string token);

public:

	// Constructors and Destructors
	Lexer();
	~Lexer();

	// Public Methods
	std::queue<std::string> getTokenFromString(std::string input);
	std::queue<std::string> getTokenFromFile(std::string path);
};

#endif