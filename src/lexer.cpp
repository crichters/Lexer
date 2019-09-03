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
						if(operators = 1){
							token += c;
							currentState = handleDoubles(tokens, token);
							operators = 0;
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

			case lineComment:
				switch(inputChar){
					case NewLine:
						token = appendToken(tokens, "NEWLINE");
						operators = 0;
						currentState = NoState;
						break;
				}
				break;	
			
			case blockComment:
				switch(inputChar){
					case Alpha:
					case Numeric:
					case Space:
					case Tab:
					case NewLine:
						operators = 0;
						token = "";
						break;

					case Special:
						if(operators == 1){
							token += c;
							tokens.push(token);
							if(!token.compare("*/")){
								currentState = NoState;
							}
							token = "";
						}
						else {
							token += c;
							operators ++;
						}
						break;
				}
				break;
		}
		
	}

	return tokens;
}

std::string Lexer::appendToken(std::queue<std::string> &tokens, std::string token){
	if(token.compare("") != 0){
		tokens.push(token);
	}
	
	return "";
}

Lexer::State Lexer::handleDoubles(std::queue<std::string> &tokens, std::string &token){
	if(!token.compare("/*")){
		token = "";
		return blockComment;
	}
	else if(!token.compare("//")){
		token = "";
		return lineComment;
	}
	else if(doubleOpsMap.find(token) == doubleOpsMap.end()){
		tokens.push(token.substr(0,1));
		tokens.push(token.substr(1));
	}
	else {
		tokens.push(token);
	}
	token = "";
	return NoState;
}

void Lexer::throwError(std::string errorMessage){
	errorMessage += '\n';
	fprintf(stderr, errorMessage.c_str());
	exit(1);
}

// End LEXER _ Private