#include "include/lexer.h"

// Begin LEXER _ PUBLIC

Lexer::Lexer(){
}

Lexer::~Lexer(){
}

std::queue<std::string> Lexer::getTokenFromString(std::string input){
	return this->parseString(input);
}

std::queue<std::string> Lexer::getTokenFromFile(std::string path){

	if(FILE *file = fopen(path.c_str(), "r")){
		fclose(file);
	} else {
		throwError("Error, cannot open file \"" + path + "\"");
	}

	std::ifstream fileBuffer(path);
	std::string fileText((std::istreambuf_iterator<char>(fileBuffer)), std::istreambuf_iterator<char>());

	return this->parseString(fileText);
}

uint Lexer::testPairs(int x, int y){
	return hashPairs(x, y);
}


// End LEXER _ Public


// Begin LEXER _ Private

std::queue<std::string> Lexer::parseString(std::string input){

	std::queue<std::string> tokens;
	std::string token;

	for(char c : input){

		if(c == ' '){
			tokens.push(token);
			token = "";
		} else {
			token += c;
		}
	}

	tokens.push(token);

	return tokens;
}

void Lexer::throwError(std::string errorMessage){
	errorMessage += '\n';
	fprintf(stderr, errorMessage.c_str());
	exit(1);
}

uint Lexer::hashPairs(int x, int y){
	return pow(2, 16) * x + y;
}

// End LEXER _ Private