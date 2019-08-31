#ifndef LEXER_H
#define LEXER_H

#include <queue>
#include <string>
#include <fstream>
#include <streambuf>
#include <cstdio>
#include <cmath>
#include <cctype>

// DEBUG
//#include <iostream>
// END DEBUG

constexpr uint LXhashPairs(int x, int y){
		return pow(2, 16) * x + y;
}


class Lexer {

	std::queue<std::string> parseString(std::string input);
	void throwError(std::string errorMessage);
	



public:

	// Constructors and Destructors
	Lexer();
	~Lexer();

	// Public Methods
	std::queue<std::string> getTokenFromString(std::string input);
	std::queue<std::string> getTokenFromFile(std::string path);
};

#endif