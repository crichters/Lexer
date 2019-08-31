#ifndef LEXER_H
#define LEXER_H

#include <queue>
#include <string>
#include <fstream>
#include <streambuf>
#include <cstdio>
#include <cmath>

// DEBUG
//#include <iostream>
// END DEBUG

class Lexer {

	std::queue<std::string> parseString(std::string input);
	void throwError(std::string errorMessage);
	uint hashPairs(int x, int y);
	enum State {};
	enum inputChar { alpha, numeric, space, tab, special };


public:

	// Constructors and Destructors
	Lexer();
	~Lexer();

	// Public Methods
	std::queue<std::string> getTokenFromString(std::string input);
	std::queue<std::string> getTokenFromFile(std::string path);

	//DEBUG
	uint testPairs(int x, int y);
	//END DEBUG
};

#endif