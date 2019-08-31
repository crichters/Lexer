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

// End LEXER _ Public


// Begin LEXER _ Private

std::queue<std::string> Lexer::parseString(std::string input){


	std::queue<std::string> tokens;
	std::string token = "";

	enum State {NoState, inWord, inNum, oneSpace, twoSpace, threeSpace, fourSpace};
	enum characterType { NoType, Alpha, Numeric, Space, Tab, Special, NewLine };

	characterType inputChar = NoType;
	State currentState = NoState;

	for(char c : input){

		// Check for Character Type
		if(isalpha(c)){
			inputChar = Alpha;
		} 
		else if(isdigit(c)){
			inputChar = Numeric;
		} 
		else if(isgraph(c)){
			inputChar = Special;
		}
		else {
			inputChar = NoType;
		}

		// Adjust Tokens and State depending on table lookup.
		switch(LXhashPairs(currentState, inputChar)){
			
			// BEGIN NoState checks
			case LXhashPairs(NoState, Alpha):
				token += c;
				currentState = inWord;
				break;

			case LXhashPairs(NoState, Numeric):
				token += c;
				currentState = inNum;
				break;

			case LXhashPairs(NoState, Space):
				currentState = oneSpace;
				break;

			case LXhashPairs(NoState, Tab):
				tokens.push("INDENT");
				break;

			case LXhashPairs(NoState, Special):
				tokens.push(c);
				break;

			case LXhashPairs(NoState, NewLine):
				tokens.push("NEWLINE");
				break;		
			// END NoState checks				

			// BEGIN inWord checks
			case LXhashPairs(inWord, Alpha):
			case LXhashPairs(inWord, Numeric):
				token += c;
				break;

			case LXhashPairs(inWord, Space):
				tokens.push(token);
				token = "";
				currentState = oneSpace;
				break;

			case LXhashPairs(inWord, Tab):
				tokens.push(token);
				tokens.push("INDENT");
				token = "";	
				currentState = NoState;
				break;

			case LXhashPairs(inWord, Special):
				tokens.push(token);
				tokens.push(c);
				token = "";
				currentState = NoState;
				break;

			case LXhashPairs(inWord, NewLine):
				tokens.push(token);
				tokens.push("NEWLINE");
				token = "";	
				currentState = NoState;
				break;	
			// END inWord checks

			// BEGIN inNum checks
			case LXhashPairs(inNum, Alpha):
				// THROW ERROR
				break;

			case LXhashPairs(inNum, Numeric):
				token += c;
				break;

			case LXhashPairs(inNum, Space):
				tokens.push(token);
				token = "";
				currentState = oneSpace;
				break;		

			case LXhashPairs(inNum, Tab):
				tokens.push(token);
				tokens.push("INDENT");
				token = "";
				currentState = NoState;
				break;

			case LXhashPairs(inNum, Special):
				tokens.push	
			// END inNum checks	

			// BEGIN oneSpace checks
			case LXhashPairs(oneSpace, Alpha):
				token += c;
				currentState = inWord;
				break;

			case LXhashPairs(oneSpace, Numeric):
				token += c;
				currentState = inNum;
				break;	

			case LXhashPairs(oneSpace, Space):
				currentState = twoSpace;
				break;	
			// END oneSpace checks	

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

// End LEXER _ Private