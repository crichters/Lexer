#include "include/lexer.h"

// Begin LEXER _ PUBLIC

Lexer::Lexer(){
	for(std::string op : doubleOps){
		doubleOpsMap[op] = 1;
	}
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

	int spaces = 0;
	int operators = 0;

	std::queue<std::string> tokens;
	std::string token = "";

	

	characterType inputChar = NoType;
	State currentState = NoState;

	for(char c : input){

		std::string charString(1, c);

		if(c == Space || c == Tab || c == NewLine){

			inputChar = (characterType)c;
		}
		else {

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
		}

		switch(currentState){
			case NoState:
				switch(inputChar){
					case Alpha:
						token += c;
						currentState = inWord;
						break;

					case Numeric:
						token += c;
						currentState = inNum;
						break;

					case Special:
						token += c;
						operators ++;
						currentState = inOp;
						break;

					case Space:
						currentState = inSpace;
						break;

					case Tab:
						appendToken(tokens, "INDENT");
						break;

					case NewLine:
						appendToken(tokens, "NEWLINE");
						break;
				}
				break;

			case inWord:
				switch(inputChar){
					case Alpha:
					case Numeric:
						token += c;
						break;

					case Special:
						token = appendToken(tokens, token);
						token += c;
						operators ++;
						currentState = inOp;
						break;

					case Space:
						token = appendToken(tokens, token);
						currentState = inSpace;
						break;

					case Tab:
						appendToken(tokens, token);
						token = appendToken(tokens, "INDENT");
						currentState = NoState;
						break;

					case NewLine:
						appendToken(tokens, token);
						token = appendToken(tokens, "NEWLINE");
						currentState = NoState;
						break;
				}
				break;

			case inNum:

				switch(inputChar){
					case Alpha:
						//THROW ERROR
						break;

					case Numeric:
						token += c;
						break;

					case Special:
						token = appendToken(tokens, token);
						token += c;
						operators ++;
						currentState = inOp;
						break;

					case Space:
						token = appendToken(tokens, token);
						currentState = inSpace;
						break;

					case Tab:
						appendToken(tokens, token);
						token = appendToken(tokens, "INDENT");
						currentState = NoState;
						break;

					case NewLine:
						appendToken(tokens, token);
						token = appendToken(tokens, "NEWLINE");
						currentState = NoState;
						break;
				}
				break;

			case inSpace:

				switch(inputChar){
					case Alpha:
						spaces = 0;
						token += c;
						currentState = inWord;
						break;

					case Numeric:
						spaces = 0;
						token += c;
						currentState = inNum;
						break;

					case Special:
						spaces = 0;
						operators ++;
						token += c;
						currentState = inOp;
						break;

					case Space:
						if(spaces >= 3){
							spaces = 0;
							appendToken(tokens, "INDENT");
						}
						else {
							spaces ++;
						}
						break;

					case Tab:
						spaces = 0;
						appendToken(tokens, "INDENT");
						currentState = NoState;
						break;

					case NewLine:
						spaces = 0;
						appendToken(tokens, "NEWLINE");
						currentState = NoState;
						break;
				}
				break;

			case inOp:

				switch(inputChar){
					case Alpha:
						token = appendToken(tokens, token);
						token += c;
						operators = 0;
						currentState = inWord;
						break;

					case Numeric:
						token = appendToken(tokens, token);
						token += c;
						operators = 0;
						currentState = inNum;
						break;

					case Special:
						if(operators = 2){
							token += c;
							token = appendToken(tokens, token);
							operators = 0;
							currentState = NoState;
						}
						else {
							token += c;
							operators ++;
						}
						break;

					case Space:
						token = appendToken(tokens, token);
						operators = 0;
						spaces ++;
						currentState = inSpace;
						break;

					case Tab:
						appendToken(tokens, token);
						token = appendToken(tokens, "INDENT");
						operators = 0;
						currentState = NoState;
						break;

					case NewLine:
						appendToken(tokens, token);
						token = appendToken(tokens, "NEWLINE");
						operators = 0;
						currentState = NoState;
						break;
				}
				break;
		}
		
	}

	return tokens;
}

std::string Lexer::appendToken(std::queue<std::string> &tokens, std::string token){
	if(token.compare("") != 0){
		if(!isalpha(token[0]) && !isdigit(token[0]) && isgraph(token[0]) && token.size() == 2){
			if(doubleOpsMap.find(token) == doubleOpsMap.end()){
				tokens.push(token.substr(0,1));
				tokens.push(token.substr(1));
			} else {
				tokens.push(token);
			}
		}
		else {
			tokens.push(token);
		}
	}
	

	return "";
}

void Lexer::throwError(std::string errorMessage){
	errorMessage += '\n';
	fprintf(stderr, errorMessage.c_str());
	exit(1);
}

// End LEXER _ Private